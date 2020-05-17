#include <Game.hpp>

#include <State.hpp>

#include <actions/Actions.hpp>

#include <board/factory/BoardFactory.hpp>

#include <utility/NumberGenerator.hpp>

#include <algorithm>

Game::Game(int numberOfPlayers, std::unique_ptr<NumberGenerator> && numberGenerator)
	: m_numberGenerator(std::move(numberGenerator))
	, m_state(std::make_unique<InitialSettlementState>())
	, m_board()
	, m_dice(*m_numberGenerator)
	, m_developmentStock(*m_numberGenerator)
	, m_longestRoad(5)
	, m_strongestArmy(3)
	, m_activePlayer(0)
	, m_gameEnded(false)
{
	setupBoard();
	setupPlayers(numberOfPlayers);
	initalizeDevelopmentStock();
}

Game::Game(int numberOfPlayers)
	: Game(numberOfPlayers, std::make_unique<RandomNumberGenerator>())
{
}

Game::~Game()
{
}

bool Game::placeInitialSetlementRoad(int settlementPosition, int roadPosition)
{
	InitialSettlementState * initialSettlementState = dynamic_cast<InitialSettlementState *>(m_state.get());
	if (!initialSettlementState)
		return false;

	PlaceInitialSettlementRoadAction intialSettlementRoadAction(m_players.at(m_activePlayer), settlementPosition, roadPosition);
	initialSettlementState->preProcessAction(intialSettlementRoadAction); // TODO: check if can do better; "dyanmic_cast"..

	return processAction(intialSettlementRoadAction);
}

bool Game::placeSettlement(int position)
{
	PlaceSettlementAction action(m_players.at(m_activePlayer), position);
	
	return processAction(action);
}

bool Game::placeRoad(int position)
{
	PlaceRoadAction action(m_players.at(m_activePlayer), position, m_longestRoad);

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

bool Game::moveRobber(int cellPosition, int vertexPosition)
{
	MoveRobberAction moveRobberAction(m_players.at(m_activePlayer), cellPosition, vertexPosition, m_players, *m_numberGenerator);

	return processAction(moveRobberAction);
}

bool Game::burnCards(const std::unordered_map<int, int> & ressourcesToBurn)
{
	CardBurnAction cardBurnAction(m_players.at(m_activePlayer), ressourcesToBurn);

	return processAction(cardBurnAction);
}

bool Game::buyDevelopmentCard()
{
	BuyDevelopmentAction buyDevelopmentAction(m_players.at(m_activePlayer), m_developmentStock);

	return processAction(buyDevelopmentAction);
}

bool Game::useDevelopmentCard(const card::DevelopmentData & developmentData)
{
	UseDevelopmentAction useDevelopmentAction(m_players.at(m_activePlayer), developmentData, m_strongestArmy);

	return processAction(useDevelopmentAction);
}

bool Game::rollDice()
{
	RollDice action(m_dice, m_players, m_activePlayer);

	return processAction(action);
}

bool Game::done()
{
	Done action(*this);

	return processAction(action);
}

bool Game::gameEnded() const
{
	return m_gameEnded;
}

std::vector<ActionType> Game::getPossibleActions() const
{
	return m_state.get()->getPossibleActions();
}

int Game::getPlayerCount() const
{
	return m_players.size();
}

int Game::getActivePlayerId() const
{
	return m_activePlayer;
}

std::string Game::getBoardInfo() const
{
	return m_board.serialize();
}

std::string Game::getPlayersInfo() const
{
	return serialize::containerSerialize(m_players, std::string(), std::string("Players: \n"), std::string("\n"));
}

int Game::getDiceValue() const
{
	return m_dice.getValue();
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

void Game::setState(std::unique_ptr<State> && state)
{
	m_state = std::move(state);
}

void Game::setupBoard()
{
	board::BoardFactory factory;
	m_board = factory.generateBoard();
}

void Game::setupPlayers(int numberOfPlayers)
{
   m_players.reserve(numberOfPlayers);
   for(int i = 0; i < numberOfPlayers; ++i)
      m_players.push_back(player::Player(i));
}

void Game::initalizeDevelopmentStock()
{
	m_developmentStock.initialize(m_players, m_board, m_longestRoad, m_strongestArmy);
}

bool Game::processAction(Action & action)
{
	// TODO: This whole code could be in State to assure allignement in calls. Board in construcotr of action

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

namespace builder {

std::unique_ptr<GameInterface> buildGame(int numberOfPlayer, std::unique_ptr<NumberGenerator> && numberGenerator)
{
	return std::make_unique<Game>(numberOfPlayer, std::move(numberGenerator));
}

}