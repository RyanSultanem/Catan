#include "../Catan/GameInterface.hpp"

#include "../Catan/Actions.hpp" // TODO: hack. should be removed. should only have access to ActionType

#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>


bool reactionInitialSettlementRoad(GameInterface & game)
{
	int settlementPosition = -1;
	int roadPosition = -1;

	std::cout << "Settlement Position: ";
	std::cin >> settlementPosition;

	std::cout << "Road Position: ";
	std::cin >> roadPosition;

	return game.placeInitialSetlementRoad(settlementPosition, roadPosition);
}

bool reactionPlaceSettlement(GameInterface & game)
{
	int settlementPosition = -1;

	std::cout << "Settlement Position: ";
	std::cin >> settlementPosition;

	return game.placeSettlement(settlementPosition);
}

bool reactionPlaceRoad(GameInterface & game)
{
	int roadPosition = -1;

	std::cout << "Road Position: ";
	std::cin >> roadPosition;

	return game.placeRoad(roadPosition);
}

bool reactionRollDice(GameInterface & game)
{
	return game.rollDice();
}

bool reactionDone(GameInterface & game)
{
	return game.done();
}

bool reactionPlaceCity(GameInterface & game)
{
	int cityPosition = -1;

	std::cout << "City Position: ";
	std::cin >> cityPosition;

	return game.placeCity(cityPosition);
}

bool reactionExchangeCard(GameInterface & /*game*/)
{
	return false;
}

bool reactionMoveRobber(GameInterface & game)
{
	int newRobberPosition = -1;
	int vertexSteal = -1;

	std::cout << "Robber Position: ";
	std::cin >> newRobberPosition;

	std::cout << "Vertex Position: ";
	std::cin >> vertexSteal;

	return game.moveRobber(newRobberPosition, vertexSteal);
}

bool reactionCardBurn(GameInterface & /*game*/)
{
	return false;
}

bool reactionBuyDevelopment(GameInterface & game)
{
	return game.buyDevelopmentCard();
}

bool reactionUseDevelopment(GameInterface & /*game*/)
{
	return false;
}

std::unordered_map<ActionType, std::function<bool(GameInterface&)>> actionReactions = 
{
	{ ActionType::PlaceInitialSettlementRoad,	reactionInitialSettlementRoad },
	{ ActionType::PlaceSettlement,				reactionPlaceSettlement },
	{ ActionType::PlaceRoad,					reactionPlaceRoad },
	{ ActionType::RollDice,						reactionRollDice },
	{ ActionType::Done,							reactionDone },
	{ ActionType::PlaceCity,					reactionPlaceCity },
	{ ActionType::ExchangeCards,				reactionExchangeCard },
	{ ActionType::MoveRobber,					reactionMoveRobber },
	{ ActionType::CardBurn,						reactionCardBurn },
	{ ActionType::BuyDevelopment,				reactionBuyDevelopment },
	{ ActionType::UseDevelopment,				reactionUseDevelopment }
};


std::unordered_map<ActionType, std::string> actionChoice = 
{
	{ActionType::PlaceInitialSettlementRoad,	"Initial Settlement/Road"},
	{ActionType::PlaceSettlement,				"Place Settlement"},
	{ActionType::PlaceRoad,						"Place Road"},
	{ActionType::RollDice,						"Roll Dice"},
	{ActionType::Done,							"Done"},
	{ActionType::PlaceCity,						"Place City"},
	{ActionType::ExchangeCards,					"Exchange Cards"},
	{ActionType::MoveRobber,					"Move Robber"},
	{ActionType::CardBurn,						"Card Burn"},
	{ActionType::BuyDevelopment,				"Buy Development"},
	{ActionType::UseDevelopment,				"Use Development"}
};

void displayOptions(const std::vector<ActionType> & possibleActions)
{
	std::cout << "Possible Actions: " << std::endl;

	int choiceNumber = 0;
	std::for_each(possibleActions.begin(), possibleActions.end(),
		[&choiceNumber](ActionType actionType)
	{
		std::cout << choiceNumber << ". " << actionChoice.at(actionType) << std::endl;
		++choiceNumber;
	});
}

int getActionChoice()
{
	int chosenNumber = -1;
	std::cin >> chosenNumber;

	return chosenNumber;
}

void update(GameInterface & game)
{
	std::vector<ActionType> possibleActions = game.getPossibleActions();
	
	int chosenNumber = -1;
	while (chosenNumber < 0 || chosenNumber >= possibleActions.size())
	{
		displayOptions(possibleActions);
		chosenNumber = getActionChoice();
	}

	ActionType chosenAction = possibleActions.at(chosenNumber);

	bool actionSuccess = actionReactions[chosenAction](game);

	std::cout << std::string(actionSuccess ? "Succss" : "Failed") << std::endl;
}

void displayGameInfo(GameInterface & /*game*/)
{
	std::cout << std::endl;
}

int main()
{
	std::unique_ptr<GameInterface> gamePtr = builder::buildGame(3);
	GameInterface & game = *gamePtr;

	while(!game.gameEnded())
	{
		update(game);
		displayGameInfo(game);
	}

	system("PAUSE");
	return 0;
}