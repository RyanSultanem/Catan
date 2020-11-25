#include <GameInterface.hpp>

#include <actions/Actions.hpp> // TODO: hack. should be removed. should only have access to ActionType
#include <card/Card.hpp>
#include <card/Development.hpp>
#include <utility/NumberGenerator.hpp>

#include <ctime>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class InputNumberGenerator : public NumberGenerator
{
public:
	InputNumberGenerator()
	{
		srand(time(0));
	}

	int generateNumber(int min, int max) const override
	{
		int number = 0;
		do
		{
			std::cout << "Choose a number between " << min << " and " << max << ":";
			std::cin >> number;

		} while((number < min || number > max) && (number != 0));

		if(number == 0)
			number = rand()%max + min;

		return number;
	}
};

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

bool reactionExchangeCard(GameInterface & game)
{
   int changeType = -1;
   int resultType = -1;

   std::cout << "Change Type: ";
   std::cin >> changeType;

   std::cout << "Result Type: ";
   std::cin >> resultType;

	return game.exchangeCards(resultType, changeType);
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

bool reactionCardBurn(GameInterface & game)
{
   int choice = 0;
   std::unordered_map<int, int> ressources(5);

   while (true)
   {
      std::cout << "Choose Ressource to Burn (-1 to stop): ";
      std::cin >> choice;

      if (choice == -1)
         break;

      int value = 1;
      if (ressources.find(choice) != ressources.end())
         value += ressources.at(choice);

      ressources.insert_or_assign(choice, value);
   }

	return game.burnCards(ressources);
}

bool reactionBuyDevelopment(GameInterface & game)
{
	return game.buyDevelopmentCard();
}

void fillKnightInfo(card::DevelopmentData & data)
{
	// Factorise with moveRobber;

	int newRobberPosition = -1;
	int vertexSteal = -1;

	std::cout << "Robber Position: ";
	std::cin >> newRobberPosition;

	std::cout << "Vertex Position: ";
	std::cin >> vertexSteal;

	data.setCellVertexPosition({ newRobberPosition , vertexSteal});
	data.setDevelopmentType(card::DevelopmentType::Knight);
}

void fillMonopolyRessource(card::DevelopmentData & data)
{
	std::string ressource;
	std::cout << "Choose a Ressource (Monopoly): L, B, W, G, O" << std::endl;
	std::cin >> ressource;

	card::Ressource type;
	if(ressource == "L")
		type = card::Ressource::LUMBER;
	else if (ressource == "B")
		type = card::Ressource::BRICK;
	else if (ressource == "W")
		type = card::Ressource::WOOL;
	else if (ressource == "G")
		type = card::Ressource::GRAIN;
	else if (ressource == "O")
		type = card::Ressource::ORE;
	else
		return;

	data.setMonopolyRessource(type);
	data.setDevelopmentType(card::DevelopmentType::Monopoly);
}

void fillFreeRoads(card::DevelopmentData & data)
{
	int roadPosition1 = -1;

	std::cout << "Road Position 1: ";
	std::cin >> roadPosition1;

	int roadPosition2 = -1;

	std::cout << "Road Position 2: ";
	std::cin >> roadPosition2;

	data.setRoadPosition({roadPosition1, roadPosition2});
	data.setDevelopmentType(card::DevelopmentType::BuildTwoFreeRoads);
}

void fillFreeRessources(card::DevelopmentData & data)
{
	std::string ressource1;
	std::cout << "Choose a Ressource (Monopoly): L, B, W, G, O" << std::endl;
	std::cin >> ressource1;

	card::Ressource type;
	if (ressource1 == "L")
		type = card::Ressource::LUMBER;
	else if (ressource1 == "B")
		type = card::Ressource::BRICK;
	else if (ressource1 == "W")
		type = card::Ressource::WOOL;
	else if (ressource1 == "G")
		type = card::Ressource::GRAIN;
	else if (ressource1 == "O")
		type = card::Ressource::ORE;
	else
		return;

	std::string ressource2;
	std::cout << "Choose a Ressource (Monopoly): L, B, W, G, O" << std::endl;
	std::cin >> ressource2;

	card::Ressource type2;
	if (ressource2 == "L")
		type2 = card::Ressource::LUMBER;
	else if (ressource2 == "B")
		type2 = card::Ressource::BRICK;
	else if (ressource2 == "W")
		type2 = card::Ressource::WOOL;
	else if (ressource2 == "G")
		type2 = card::Ressource::GRAIN;
	else if (ressource2 == "O")
		type2 = card::Ressource::ORE;
	else
		return;

	data.setFreeRessources({type, type2});
	data.setDevelopmentType(card::DevelopmentType::FreeRessources);
}

bool reactionUseDevelopment(GameInterface & game)
{
	std::string choice;
	std::cout << "Choose Development Type: (Knight: K; FreeRessources: F; BuildRoads: B; Monopoly: M; Victory: V)" << std::endl;
	std::cin >> choice;

	card::DevelopmentData data;

	if (choice == "K")
	{
		fillKnightInfo(data);
	}
	else if (choice == "F")
	{
		fillFreeRessources(data);
	}
	else if (choice == "B")
	{
		fillFreeRoads(data);
	}
	else if (choice == "M")
	{
		fillMonopolyRessource(data);
	}
	else if (choice == "V")
	{
		data.setDevelopmentType(card::DevelopmentType::VictoryPoint);
	}
	else
		return false;

	return game.useDevelopmentCard(data);
}

std::unordered_map<ActionType, std::function<bool(GameInterface&)>> actionReactions = 
{
	{ ActionType::PlaceInitialSettlementRoad,	reactionInitialSettlementRoad },
	{ ActionType::PlaceSettlement,				reactionPlaceSettlement },
	{ ActionType::PlaceRoad,					   reactionPlaceRoad },
	{ ActionType::RollDice,						   reactionRollDice },
	{ ActionType::Done,							   reactionDone },
	{ ActionType::PlaceCity,					   reactionPlaceCity },
	{ ActionType::ExchangeCards,				   reactionExchangeCard },
	{ ActionType::MoveRobber,					   reactionMoveRobber },
	{ ActionType::CardBurn,						   reactionCardBurn },
	{ ActionType::BuyDevelopment,				   reactionBuyDevelopment },
	{ ActionType::UseDevelopment,				   reactionUseDevelopment }
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

   std::cout << "Choose an Action: ";
	std::cin >> chosenNumber;

	return chosenNumber;
}

void displayActivePlayer(GameInterface & game)
{
   std::cout << std::endl;
   std::cout << "Active Player: " << game.getActivePlayerId() << std::endl;
}

void update(GameInterface & game)
{
	std::vector<ActionType> possibleActions = game.getPossibleActions();
	
	int chosenNumber = -1;
	while (chosenNumber < 0 || chosenNumber >= possibleActions.size())
	{
		displayOptions(possibleActions);
		displayActivePlayer(game);
		chosenNumber = getActionChoice();
	}

	ActionType chosenAction = possibleActions.at(chosenNumber);

	bool actionSuccess = actionReactions[chosenAction](game);

	std::cout << std::string(actionSuccess ? "Succss" : "Failed") << std::endl;
	std::cout << std::endl << std::endl;
}

void displayGameInfo(const GameInterface & game)
{
	std::cout << game.getBoardInfo() << std::endl;
	std::cout << std::endl;
	std::cout << game.getPlayersInfo() << std::endl;
	std::cout << std::endl;
	std::cout << "Dice Value: " << game.getDiceValue() << std::endl;
	std::cout << std::endl;
}

int main()
{
	int numberOfPlayers = 0;
	std::cout << "Number of Players: ";
	std::cin >> numberOfPlayers;

	std::unique_ptr<InputNumberGenerator> inputNumberGenerator = std::make_unique<InputNumberGenerator>();
	std::unique_ptr<GameInterface> gamePtr = builder::buildGame(numberOfPlayers, std::move(inputNumberGenerator));
	GameInterface & game = *gamePtr;

	while(!game.gameEnded())
	{
		displayGameInfo(game);
		update(game);
	}

	system("PAUSE");
	return 0;
}