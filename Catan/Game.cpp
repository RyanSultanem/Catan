#include "Game.hpp"

#include "Actions.hpp"
#include "Dice.hpp"
#include "PlayerReactions.hpp"

#include "Building.hpp"

#include <algorithm>
#include <ctime>

Game::Game(Interface & interface, int numberOfPlayers)
	: /*m_interface(interface)
	,*/ m_activePlayer(0)
	, m_secondInitialPlacementRun(false)
{
	srand(time(NULL)); // TODO: In Dice? Chose to be here because on initialization of the game.
	setupPlayers(numberOfPlayers);
}

Game::~Game()
{
}

//void Game::play()
//{
//	//showStatus();
//
//	//initialSettlmentPlacement();
//
//	//showStatus();
//
//	playTurns();
//}

bool Game::placeInitialSetlementRoad(int settlementPosition, int roadPosition)
{
	PlaceInitialSettlementRoadAction action(m_players.at(m_activePlayer), settlementPosition, roadPosition, m_secondInitialPlacementRun);

	return processAction(action);
}

bool Game::placeSettlement(int position)
{
	PlaceSettlementAction action(m_players.at(m_activePlayer), position);
	
	return processAction(action);
}

bool Game::placeRoad(int position)
{
	PlaceRoadAction action(m_players.at(m_activePlayer), position);

	return processAction(action);
}

bool Game::placeCity(int position)
{
	PlaceCityAction action(m_players.at(m_activePlayer), position);

	return processAction(action);
}

bool Game::exchangeCards(int resultType, int typeToTrade)
{
	ExchangeCardsAction action(m_players.at(m_activePlayer), resultType, typeToTrade);

	return processAction(action);
}

bool Game::rollDice()
{
	RollDice action(m_players);

	return processAction(action);
}

bool Game::done()
{
	Done action(*this);

	return processAction(action);
}

int Game::getPlayerCount() const
{
	return m_players.size();
}

int Game::getActivePlayerId() const
{
	return m_activePlayer;
}

void Game::setNextActivePlayer()
{
	m_activePlayer = (m_activePlayer + 1) % m_players.size();
}

void Game::setNextActivePlayer(int playerId)
{
	if (playerId < m_players.size() && playerId >= 0)
		m_activePlayer = playerId;
}

void Game::setSecondInitialPlacementRun()
{
	m_secondInitialPlacementRun = true;
}

bool Game::isSecondInitialPlacementRun() const
{
	return m_secondInitialPlacementRun;
}

void Game::setState(std::unique_ptr<State> && state)
{
	m_state = std::move(state);
}

void Game::setupPlayers(int numberOfPlayers)
{
   m_players.reserve(numberOfPlayers);
   for(int i = 0; i < numberOfPlayers; ++i)
      m_players.push_back(player::Player(i));
}

bool Game::processAction(const Action & action)
{
	// TODO: This whole code could be in State to assure allignement in calls.

	if (!m_state->isValid(action))
		return false;

	bool actionSuccess = action.execute(m_board);

	if (actionSuccess)
		m_state->nextState(*this, action);
	
	return actionSuccess;
}

//void Game::showStatus()
//{
//   m_interface.showBoard(m_board.serialize());
//   m_interface.showPlayerInfos(serialize::containerSerialize(m_players, "", "Players: "));
//}

//void Game::giveRessources(int value)
//{
	//const std::vector<cell::CellRef> activeCells = m_board.getCellsWithNumber(value);

	//std::for_each(activeCells.begin(), activeCells.end(),
	//	[this](const cell::Cell & activeCell)
	//{
	//	const std::vector<token::building::Building*> activeBuildings = activeCell.getActiveBuildings();

	//	std::for_each(activeBuildings.begin(), activeBuildings.end(),
	//		[this, &activeCell](const token::building::Building * activeBuilding)
	//	{
	//		// TODO: check if reference is equivalent to position in vector
	//		player::Player & player = m_players.at(activeBuilding->getReference());
	//		player.addRessource(activeCell.produceLandRessource().getType(), activeBuilding->getPoints());
	//	});
	//});
//}

//void Game::playTurns()
//{
//	board::Dice dice;
//	int turnCount = 0;
//
//	//while(!m_gameEnded)
//	{
//		int diceValue = dice.roll().getValue();
//
//		giveRessources(diceValue);
//
//		player::Player & activePlayer = m_players.at(turnCount % m_players.size());
//
//		++turnCount;
//	}
//}
