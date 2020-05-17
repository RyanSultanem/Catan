#include <card/Development.hpp>

#include <Achievement.hpp>

#include <board/Board.hpp>
#include <board/factory/BoardFactory.hpp>

#include <mock/PlaceConditionMock.hpp>

#include <player/Player.hpp>

#include <gtest/gtest.h>

TEST(BuildTwoFreeRoads, SimpleWorking)
{
	board::Board board = board::BoardFactory().generateBoard();
	Achievement longestRoad(5);
	card::BuildTwoFreeRoadsAction action(board, longestRoad);

	player::Player player(0);
	PlaceRoadConditionMock trueConditionMocked(player.getId());

	board.placeRoad(0, *player.getRoad(), trueConditionMocked);

	int firstPosition = 1;
	int secondPosition = 2;
	card::DevelopmentData data;
	data.setRoadPosition({ firstPosition, secondPosition });

	bool result = action.execute(player, data);

	EXPECT_TRUE(result);
	EXPECT_TRUE(board.getEdgeAt(firstPosition).value().get().hasRoadOfPlayer(player.getId()));
	EXPECT_TRUE(board.getEdgeAt(secondPosition).value().get().hasRoadOfPlayer(player.getId()));
}

TEST(BuildTwoFreeRoads, SimpleWorking_InverseOrder)
{
	board::Board board = board::BoardFactory().generateBoard();
	Achievement longestRoad(5);
	card::BuildTwoFreeRoadsAction action(board, longestRoad);

	player::Player player(0);
	PlaceRoadConditionMock trueConditionMocked(player.getId());

	board.placeRoad(0, *player.getRoad(), trueConditionMocked);

	int firstPosition = 2;
	int secondPosition = 1;
	card::DevelopmentData data;
	data.setRoadPosition({ firstPosition, secondPosition });

	bool result = action.execute(player, data);

	EXPECT_TRUE(result);
	EXPECT_TRUE(board.getEdgeAt(firstPosition).value().get().hasRoadOfPlayer(player.getId()));
	EXPECT_TRUE(board.getEdgeAt(secondPosition).value().get().hasRoadOfPlayer(player.getId()));
}

TEST(BuildTwoFreeRoads, OneOnAlreadyPlaced)
{
	board::Board board = board::BoardFactory().generateBoard();
	Achievement longestRoad(5);
	card::BuildTwoFreeRoadsAction action(board, longestRoad);

	player::Player player(0);
	PlaceRoadConditionMock trueConditionMocked(player.getId());

	board.placeRoad(0, *player.getRoad(), trueConditionMocked);

	int firstPosition = 0;
	int secondPosition = 1;
	card::DevelopmentData data;
	data.setRoadPosition({ firstPosition, secondPosition });

	bool result = action.execute(player, data);

	EXPECT_FALSE(result);
	EXPECT_TRUE(board.getEdgeAt(firstPosition).value().get().hasRoadOfPlayer(player.getId()));
	EXPECT_FALSE(board.getEdgeAt(secondPosition).value().get().hasRoadOfPlayer(player.getId()));
}

TEST(BuildTwoFreeRoads, OneOnAlreadyPlaced_Inverse)
{
	board::Board board = board::BoardFactory().generateBoard();
	Achievement longestRoad(5);
	card::BuildTwoFreeRoadsAction action(board, longestRoad);

	player::Player player(0);
	PlaceRoadConditionMock trueConditionMocked(player.getId());

	board.placeRoad(0, *player.getRoad(), trueConditionMocked);

	int firstPosition = 1;
	int secondPosition = 0;
	card::DevelopmentData data;
	data.setRoadPosition({ firstPosition, secondPosition });

	bool result = action.execute(player, data);

	EXPECT_FALSE(result);
	EXPECT_FALSE(board.getEdgeAt(firstPosition).value().get().hasRoadOfPlayer(player.getId()));
	EXPECT_TRUE(board.getEdgeAt(secondPosition).value().get().hasRoadOfPlayer(player.getId()));
}

TEST(BuildTwoFreeRoads, BothOnAlreadyPlaced)
{
	board::Board board = board::BoardFactory().generateBoard();
	Achievement longestRoad(5);
	card::BuildTwoFreeRoadsAction action(board, longestRoad);

	player::Player player(0);
	PlaceRoadConditionMock trueConditionMocked(player.getId());

	board.placeRoad(0, *player.getRoad(), trueConditionMocked);
	board.placeRoad(1, *player.getRoad(), trueConditionMocked);

	int firstPosition = 0;
	int secondPosition = 1;
	card::DevelopmentData data;
	data.setRoadPosition({ firstPosition, secondPosition });

	bool result = action.execute(player, data);

	EXPECT_FALSE(result);
	EXPECT_TRUE(board.getEdgeAt(firstPosition).value().get().hasRoadOfPlayer(player.getId()));
	EXPECT_TRUE(board.getEdgeAt(secondPosition).value().get().hasRoadOfPlayer(player.getId()));
}

TEST(BuildTwoFreeRoads, NotConnectedToInitialRoad)
{
	board::Board board = board::BoardFactory().generateBoard();
	Achievement longestRoad(5);
	card::BuildTwoFreeRoadsAction action(board, longestRoad);

	player::Player player(0);
	PlaceRoadConditionMock trueConditionMocked(player.getId());

	board.placeRoad(0, *player.getRoad(), trueConditionMocked);

	int firstPosition = 2;
	int secondPosition = 3;
	card::DevelopmentData data;
	data.setRoadPosition({ firstPosition, secondPosition });

	bool result = action.execute(player, data);

	EXPECT_FALSE(result);
	EXPECT_FALSE(board.getEdgeAt(firstPosition).value().get().hasRoadOfPlayer(player.getId()));
	EXPECT_FALSE(board.getEdgeAt(secondPosition).value().get().hasRoadOfPlayer(player.getId()));
}

TEST(VictoryPont, OneVictory)
{
	card::VictoryPointAction action;

	player::Player player(0);

	int initialPoints = player.getPoints();
	action.execute(player, card::DevelopmentData());

	int afterDevPoints = player.getPoints();
	EXPECT_EQ(afterDevPoints - initialPoints, 1);
}

TEST(Monopoly, AllMonopolyRessourceSteal)
{
	std::vector<player::Player> players;
	players.emplace_back(0);
	players.emplace_back(1);
	players.emplace_back(2);

	card::MonopolyAction action(players);

	players[0].addRessource(card::RessourceType::BRICK, 3);
	players[1].addRessource(card::RessourceType::BRICK, 5);
	players[2].addRessource(card::RessourceType::BRICK, 7);

	card::DevelopmentData data;
	data.setMonopolyRessource(card::RessourceType::BRICK);

	action.execute(players[0], data);

	EXPECT_EQ(players[0].getRessourceCount(card::RessourceType::BRICK), 15);
	EXPECT_EQ(players[1].getRessourceCount(card::RessourceType::BRICK), 0);
	EXPECT_EQ(players[2].getRessourceCount(card::RessourceType::BRICK), 0);
}

TEST(Monopoly, DoesNot_Affect_NonMonopolyResssource)
{
	std::vector<player::Player> players;
	players.emplace_back(0);
	players.emplace_back(1);

	card::MonopolyAction action(players);

	players[0].addRessource(card::RessourceType::BRICK, 3);
	players[1].addRessource(card::RessourceType::BRICK, 5);

	players[0].addRessource(card::RessourceType::WOOL, 2);
	players[1].addRessource(card::RessourceType::WOOL, 1);

	card::DevelopmentData data;
	data.setMonopolyRessource(card::RessourceType::BRICK);

	action.execute(players[0], data);

	EXPECT_EQ(players[0].getRessourceCount(card::RessourceType::WOOL), 2);
	EXPECT_EQ(players[1].getRessourceCount(card::RessourceType::WOOL), 1);
}

TEST(FreeRessources, AddingTwoDifferentRessources)
{
	player::Player player(0);

	card::FreeRessourcesAction action;
	card::DevelopmentData data;
	data.setFreeRessources({ card::RessourceType::BRICK, card::RessourceType::LUMBER });

	int initialBrickCount = player.getRessourceCount(card::RessourceType::BRICK);
	int initialLumberCount = player.getRessourceCount(card::RessourceType::LUMBER);
	int initialWoolCount = player.getRessourceCount(card::RessourceType::WOOL);

	action.execute(player, data);

	EXPECT_EQ(player.getRessourceCount(card::RessourceType::BRICK) - initialBrickCount, 1);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::LUMBER) - initialLumberCount, 1);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::WOOL) - initialWoolCount, 0);
}

TEST(FreeRessources, AddingSameRessourceTwice)
{
	player::Player player(0);

	card::FreeRessourcesAction action;
	card::DevelopmentData data;
	data.setFreeRessources({ card::RessourceType::BRICK, card::RessourceType::BRICK });

	int initialBrickCount = player.getRessourceCount(card::RessourceType::BRICK);
	int initialLumberCount = player.getRessourceCount(card::RessourceType::LUMBER);
	int initialWoolCount = player.getRessourceCount(card::RessourceType::WOOL);

	action.execute(player, data);

	EXPECT_EQ(player.getRessourceCount(card::RessourceType::BRICK) - initialBrickCount, 2);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::LUMBER) - initialLumberCount, 0);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::WOOL) - initialWoolCount, 0);
}