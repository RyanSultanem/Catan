#include <Game.hpp>

#include <State.hpp>

#include <actions/DevelopmentAction.hpp>
#include <actions/PlaceTokenAction.hpp>
#include <actions/PlayerAction.hpp>

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
	, m_players()
	, m_winnerId(std::nullopt)
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

	PlaceInitialSettlementRoadAction intialSettlementRoadAction(m_players.getActivePlayer(), m_board, settlementPosition, roadPosition);
	initialSettlementState->preProcessAction(intialSettlementRoadAction); // TODO: check if can do better; "dyanmic_cast"..

	return processAction(intialSettlementRoadAction);
}

bool Game::placeSettlement(int position)
{
	PlaceSettlementAction action(m_players.getActivePlayer(), m_board, position);
	
	return processAction(action);
}

bool Game::placeRoad(int position)
{
	PlaceRoadAction action(m_players.getActivePlayer(), m_board, position, m_longestRoad);

	return processAction(action);
}

bool Game::placeCity(int position)
{
	PlaceCityAction action(m_players.getActivePlayer(), m_board, position);

	return processAction(action);
}

bool Game::exchangeCards(int resultType, int typeToTrade)
{
	ExchangeCardsAction action(m_players.getActivePlayer(), resultType, typeToTrade);

	return processAction(action);
}

bool Game::moveRobber(int cellPosition, int vertexPosition)
{
	MoveRobberAction moveRobberAction(m_players, m_board, cellPosition, vertexPosition, *m_numberGenerator);

	return processAction(moveRobberAction);
}

bool Game::burnCards(const std::unordered_map<int, int> & ressourcesToBurn)
{
	CardBurnAction cardBurnAction(m_players.getActivePlayer(), ressourcesToBurn);

	return processAction(cardBurnAction);
}

bool Game::buyDevelopmentCard()
{
	BuyDevelopmentAction buyDevelopmentAction(m_players.getActivePlayer(), m_developmentStock);

	return processAction(buyDevelopmentAction);
}

bool Game::useDevelopmentCard(const card::DevelopmentData & developmentData)
{
	UseDevelopmentAction useDevelopmentAction(m_players.getActivePlayer(), developmentData, m_strongestArmy);

	return processAction(useDevelopmentAction);
}

bool Game::rollDice()
{
	RollDice action(m_board, m_dice, m_players);

	return processAction(action);
}

bool Game::done()
{
	Done action;

	return processAction(action);
}

bool Game::gameEnded() const
{
	return m_winnerId.has_value();
}

std::optional<int> Game::getWinnerId() const
{
	return m_winnerId;
}

std::vector<ActionType> Game::getPossibleActions() const
{
	return m_state.get()->getPossibleActions();
}

void Game::updateGameEnd(int playerId)
{
	if (!m_winnerId)
		m_winnerId = playerId;
}

int Game::getActivePlayerId() const
{
	return m_players.getActivePlayerId();
}

std::string Game::getBoardInfo() const
{
	return m_board.serialize();
}

std::string Game::getPlayersInfo() const
{
	return serialize::containerSerialize(m_players.getPlayers(), std::string(), std::string("Players: \n"), std::string("\n"));
}

int Game::getDiceValue() const
{
	return m_dice.getValue();
}

void Game::setupBoard()
{
	board::BoardFactory factory;
	m_board = factory.generateBoard();
}

void Game::setupPlayers(int numberOfPlayers)
{
   m_players.initializePlayers(numberOfPlayers, this);
}

void Game::initalizeDevelopmentStock()
{
	m_developmentStock.initialize(m_players, m_board, m_longestRoad, m_strongestArmy);
}

bool Game::processAction(Action & action)
{
	if (!m_state->isValid(action))
		return false;

	bool actionSuccess = action.execute();

	if (actionSuccess)
		updateStateWithAction(action);
	
	return actionSuccess;
}

void Game::updateStateWithAction(Action & action)
{
	NextStateResult newStateResult = m_state->computeNextState(m_players, action);
	if (newStateResult.isUpdated())
		m_state = std::move(newStateResult.takeNewState());
}

namespace builder {

std::unique_ptr<GameInterface> buildGame(int numberOfPlayer, std::unique_ptr<NumberGenerator> && numberGenerator)
{
	return std::make_unique<Game>(numberOfPlayer, std::move(numberGenerator));
}

}