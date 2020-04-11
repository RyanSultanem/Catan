#include "Actions.hpp"

#include <Achievement.hpp>
#include <Game.hpp>

#include <board/Board.hpp>
#include <board/Dice.hpp>

#include <card/Card.hpp>
#include <card/Development.hpp>

#include <player/Player.hpp>
#include <player/PlayerReactions.hpp>

#include <token/Conditions.hpp>

#include <utility/Utility.hpp>
#include <utility/NumberGenerator.hpp>

#include <iterator>

PlaceSettlementAction::PlaceSettlementAction(player::Player & player, int position)
	: m_player(player)
	, m_position(position)
{
}

bool PlaceSettlementAction::execute(board::Board & board)
{
	bool isSuccess = preExecute();

	std::optional<token::building::SettlementRef> optSettlement = m_player.getSettlement();

	if (isSuccess && optSettlement)
	{
		isSuccess = board.placeSettlement(m_position, *optSettlement, PlaceSettlementCondition(m_player.getId()));
			
		if(isSuccess)
			postExecute(board);
	}

	return isSuccess;
}

ActionType PlaceSettlementAction::getType() const
{
	return ActionType::PlaceSettlement;
}

bool PlaceSettlementAction::preExecute() const
{
	std::optional<token::building::SettlementRef> settlmentOpt = m_player.getSettlement();
	return settlmentOpt ? player::reactions::tokenRessourcesAvailable(m_player, settlmentOpt.value()) : false;
}

namespace {

void updateHarborPlacement(player::Player & player, const board::Board & board, int settlementPosition)
{
	const std::optional<HarborCRef> & optHarbor = board.getHarbor(settlementPosition);
	if (optHarbor)
		player::reactions::settlementPlacedOnHarbor(player, *optHarbor);
}

} // namesapce anonymous

void PlaceSettlementAction::postExecute(const board::Board & board) const
{
	if (player::reactions::tokenPayRessources(m_player, m_player.getSettlement().value()))
	{
		player::reactions::tokenPlaced(m_player, m_player.getSettlement().value());
		updateHarborPlacement(m_player, board, m_position);
	}
}

PlaceInitialSettlementRoadAction::PlaceInitialSettlementRoadAction(player::Player & player, int settlementPosition, int roadPosition)
	: m_player(player)
	, m_settlementPosition(settlementPosition)
	, m_roadPosition(roadPosition)
	, m_secondRun(false)
{
}

bool PlaceInitialSettlementRoadAction::execute(board::Board & board)
{
	bool isSuccess = false;

	std::optional<token::building::SettlementRef> optSettlement = m_player.getSettlement();
	std::optional<token::RoadRef> optRoad = m_player.getRoad();

	if (optSettlement && optRoad)
	{
		// TODO: fix issue if place settlmenet is succcessful bit road isnt. preExecute with conditions?
		// TODO: check road and settlement are adjencebt before placing
		isSuccess = board.placeSettlement(m_settlementPosition, *optSettlement, PlaceInitialSettlementCondition(m_player.getId()));
		isSuccess = isSuccess && board.placeRoad(m_roadPosition, *optRoad, PlaceInitialRoadCondition(m_player.getId(), m_settlementPosition));

		if (isSuccess)
			postExecute(board);
	}
	else
		isSuccess = false;

	return isSuccess;
}

ActionType PlaceInitialSettlementRoadAction::getType() const
{
	return ActionType::PlaceInitialSettlementRoad;
}

void PlaceInitialSettlementRoadAction::setSecondRun(bool secondRun)
{
	m_secondRun = secondRun;
}

void PlaceInitialSettlementRoadAction::postExecute(const board::Board & board) const
{
	player::reactions::tokenPlaced(m_player, m_player.getSettlement().value());
	player::reactions::tokenPlaced(m_player, m_player.getRoad().value());
	
	updateHarborPlacement(m_player, board, m_settlementPosition);

	if (m_secondRun)
	{
		const std::vector<card::RessourceType> ressources = board.getRessourcesFromVertexPosition(m_settlementPosition);
		std::for_each(ressources.begin(), ressources.end(), 
			[this](const card::RessourceType ressource)
		{
			m_player.addRessource(ressource, 1);
		});
	}
}

PlaceRoadAction::PlaceRoadAction(player::Player & player, int position, Achievement & longestRoad)
	: m_player(player)
	, m_position(position)
	, m_longestRoad(longestRoad)
{
}

bool PlaceRoadAction::execute(board::Board & board)
{
	bool isSuccess = preExecute();

	std::optional<token::RoadRef> optRoad = m_player.getRoad();

	if(isSuccess && optRoad)
	{
		isSuccess = board.placeRoad(m_position, *optRoad, PlaceRoadCondition(m_player.getId()));

		if (isSuccess)
			postExecute(board);
	}
	else
		isSuccess = false;

	return isSuccess;
}

ActionType PlaceRoadAction::getType() const
{
	return ActionType::PlaceRoad;
}

bool PlaceRoadAction::preExecute() const
{
	std::optional<token::RoadRef> roadOpt = m_player.getRoad();
	return roadOpt ? player::reactions::tokenRessourcesAvailable(m_player, roadOpt.value()) : false;
}

void PlaceRoadAction::postExecute(board::Board & board) const
{
	if(player::reactions::tokenPayRessources(m_player, m_player.getRoad().value()))
	{
		player::reactions::tokenPlaced(m_player, m_player.getRoad().value());
		m_longestRoad.update(m_player, LongestRoadChecker(*board.getEdgeAt(m_position)));
	}
}

PlaceCityAction::PlaceCityAction(player::Player & player, int position)
	: m_player(player)
	, m_position(position)
{
}

bool PlaceCityAction::execute(board::Board & board)
{
	bool isSuccess = preExecute();

	std::optional<token::building::CityRef> optCity = m_player.getCity();

	if(isSuccess && optCity)
	{
		isSuccess = board.placeCity(m_position, *optCity, PlaceCityCondition(m_player.getId()));

		if (isSuccess)
			postExecute();
	}
	else
		isSuccess = false;

	return isSuccess;
}

ActionType PlaceCityAction::getType() const
{
	return ActionType::PlaceCity;
}

bool PlaceCityAction::preExecute() const
{
	std::optional<token::building::CityRef> cityOpt = m_player.getCity();
	return cityOpt ? player::reactions::tokenRessourcesAvailable(m_player, cityOpt.value()) : false;
}

void PlaceCityAction::postExecute() const
{
	if(player::reactions::tokenPayRessources(m_player, m_player.getCity().value()))
		player::reactions::tokenPlaced(m_player, m_player.getCity().value());
}

RollDice::RollDice(board::Dice & dice, std::vector<player::Player> & players, int activePlayer)
 	: m_dice(dice)
	, m_players(players)
	, m_activePlayer(activePlayer)
	, m_shouldChangeRobber(false)
	, m_shouldBurn(false)
	, m_playerBurn()
{
}

bool RollDice::execute(board::Board & board)
{
	int diceValue = m_dice.roll().getValue();

	if (diceValue == 7)
	{
		m_shouldChangeRobber = true;
		checkCardBurn();
	}

	giveRessources(board, diceValue);

	return true;
}

ActionType RollDice::getType() const
{
	return ActionType::RollDice;
}

bool RollDice::shouldChangeRobber() const
{
	return m_shouldChangeRobber;
}

bool RollDice::shouldBurn() const
{
	return m_shouldBurn;
}

const std::queue<int>& RollDice::getPlayerBurnQueue() const
{
	return m_playerBurn;
}

void RollDice::giveRessources(const board::Board & board, int diceValue) const
{
	// TODO: ...ugly
	const std::vector<cell::CellCRef> activeCells = board.getCellsWithNumber(diceValue);

	std::for_each(activeCells.begin(), activeCells.end(),
		[this](const cell::Cell & activeCell)
	{
		const std::vector<token::building::Building*> activeBuildings = activeCell.getActiveBuildings();

		std::for_each(activeBuildings.begin(), activeBuildings.end(),
			[this, &activeCell](const token::building::Building * activeBuilding)
		{
			player::Player & player = m_players.at(activeBuilding->getReference());
			player.addRessource(activeCell.produceLandRessource().getType(), activeBuilding->getPoints());
		});
	});
}

void RollDice::checkCardBurn()
{
	int numberOfPlayers = m_players.size();
	int playerIndex = m_activePlayer; 
	
	// TODO: kind of weird way to do the check.. but might be okay
	// TODO: should CERTAINLY NOT be here and expose the queue.
	do
	{
		playerIndex = (playerIndex + 1) % numberOfPlayers; // Start with next player
		if (m_players.at(playerIndex).getNumberOfRessources() >= 8) // TODO: Magic Number
		{
			m_playerBurn.push(playerIndex);
			m_shouldBurn = true;
		}
	} while (playerIndex != m_activePlayer);
}

Done::Done(Game & game)
	: m_game(game)
{		
}

bool Done::execute(board::Board & /*board*/)
{
   // TODO: chcek if this should be here: for now it is also in state change. so removed below
	//m_game.setNextActivePlayer();

	return true;
}

ActionType Done::getType() const
{
	return ActionType::Done;
}

ExchangeCardsAction::ExchangeCardsAction(player::Player & player, int typeResult, int typeToTrade)
	: m_player(player)
	, m_typeResult(typeResult)
	, m_typeToTrade(typeToTrade)
{
}

bool ExchangeCardsAction::execute(board::Board & /*board*/)
{
	return player::reactions::performExchangeCards(m_player, static_cast<card::RessourceType>(m_typeResult), static_cast<card::RessourceType>(m_typeToTrade));
}

ActionType ExchangeCardsAction::getType() const
{
	return ActionType::ExchangeCards;
}

MoveRobberAction::MoveRobberAction(player::Player & player, int cellPosition, int vertexPosition, std::vector<player::Player> & players, const NumberGenerator & numberGenerator)
	: m_player(player)
	, m_cellPosition(cellPosition)
	, m_vertexPosition(vertexPosition)
	, m_players(players)
	, m_numberGenerator(numberGenerator)
{
}

bool MoveRobberAction::execute(board::Board & board)
{
	// TODO: check cellPosition and VertexPosition are adjacent..
	bool isSucess = board.moveRobber(m_cellPosition);

	if (isSucess)
	{
		std::optional<int> playerRef = board.getVertexPlayerRef(m_vertexPosition);
		if(playerRef)
		{
			player::Player & fromPlayer = m_players.at(playerRef.value());
			int ressourceIndex = m_numberGenerator.generateNumber(1, fromPlayer.getNumberOfRessources());
			player::reactions::stealPlayerCard(m_player, fromPlayer, ressourceIndex);
		}
	}

	return isSucess;
}

ActionType MoveRobberAction::getType() const
{
	return ActionType::MoveRobber;
}

CardBurnAction::CardBurnAction(player::Player & player, const std::unordered_map<int, int> & ressourcesToBurn)
	: m_player(player)
	, m_ressourcesToBurn(ressourcesToBurn.size())
{
	std::transform(ressourcesToBurn.begin(), ressourcesToBurn.end(), std::inserter(m_ressourcesToBurn, m_ressourcesToBurn.begin()),
		[](const std::pair<int, int> & element)
	{
		return std::pair<card::RessourceType, int>(static_cast<card::RessourceType>(element.first), element.second);
	});
}

bool CardBurnAction::execute(board::Board & /*board*/)
{
	if (!preExecute())
		return false;

	/*std::unordered_map<card::RessourceType, int> realRessources(m_ressourcesToBurn.size());

	std::transform(m_ressourcesToBurn.begin(), m_ressourcesToBurn.end(), std::inserter(realRessources, realRessources.begin()),
		[](const std::pair<int, int> & element)
	{
		return std::pair<card::RessourceType, int>(static_cast<card::RessourceType>(element.first), element.second);
	});*/

	return player::reactions::burnCards(m_player, m_ressourcesToBurn);
}

ActionType CardBurnAction::getType() const
{
	return ActionType::CardBurn;
}

bool CardBurnAction::preExecute() const
{
	return m_player.getNumberOfRessources() / 2 == utility::getCount(m_ressourcesToBurn) 
		&& player::reactions::ressourcesAvailable(m_player, m_ressourcesToBurn);

}

BuyDevelopmentAction::BuyDevelopmentAction(player::Player & player, DevelopmentStock & developmentStock)
	: m_player(player)
	, m_developmentStock(developmentStock)
{
}

bool BuyDevelopmentAction::execute(board::Board & board)
{
	bool isSuccess = preExecute();

	if (isSuccess)
	{
		if(player::reactions::developmentPay(m_player))
		{	
			std::optional<card::Development> optDevelopment = m_developmentStock.drawCard();
			if (optDevelopment)
			{
				m_player.receiveDevelopment(*optDevelopment);
				isSuccess = true;
			}
		}
	}

	return isSuccess;
}

ActionType BuyDevelopmentAction::getType() const
{
	return ActionType::BuyDevelopment;
}

bool BuyDevelopmentAction::preExecute()
{
	return player::reactions::developmentRessourceAvailable(m_player) && !m_developmentStock.empty();
}

UseDevelopmentAction::UseDevelopmentAction(player::Player & player, const card::DevelopmentData & developmentData, Achievement & strongestArmy)
   : m_player(player)
   , m_developmentData(developmentData)
   , m_strongestArmy(strongestArmy)
{
}

bool UseDevelopmentAction::execute(board::Board & board)
{
    std::optional<card::DevelopmentRef> optDevelopmentCard = m_player.getUnusedDevelopment(m_developmentData.getDevelopmentType());
	if (!optDevelopmentCard)
		return false;

	card::Development & developmentCard = *optDevelopmentCard;
    bool success = developmentCard.executeAction(m_player, m_developmentData);

	if(m_developmentData.getDevelopmentType() == card::DevelopmentType::Knight)
		m_strongestArmy.update(m_player, StrongestArmyChecker(m_player));
		
    // TODO: check for longest 

	return success;
}

ActionType UseDevelopmentAction::getType() const
{
   return ActionType::UseDevelopment;
}

bool UseDevelopmentAction::validPrePlayerDecisionUse(bool developmentUsed) const
{
	return !developmentUsed && m_developmentData.getDevelopmentType() == card::DevelopmentType::Knight;
}

bool UseDevelopmentAction::validPlayerDecisionUse(bool developmentUsed) const
{
	if (m_developmentData.getDevelopmentType() == card::DevelopmentType::VictoryPoint)
		return true;

	return !developmentUsed;
}
