#include <card/Development.hpp>

#include <Achievement.hpp>

#include <board/Board.hpp>
#include <board/factory/BoardFactory.hpp>

#include <mock/PlaceConditionMock.hpp>

#include <Players.hpp>
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
	Players players;
	players.initializePlayers(3, nullptr);

	card::MonopolyAction action(players);

	players.getPlayer(0).addRessource(card::Ressource::BRICK, 3);
	players.getPlayer(1).addRessource(card::Ressource::BRICK, 5);
	players.getPlayer(2).addRessource(card::Ressource::BRICK, 7);

	card::DevelopmentData data;
	data.setMonopolyRessource(card::Ressource::BRICK);

	action.execute(players.getPlayer(0), data);

	EXPECT_EQ(players.getPlayer(0).getRessourceCount(card::Ressource::BRICK), 15);
	EXPECT_EQ(players.getPlayer(1).getRessourceCount(card::Ressource::BRICK), 0);
	EXPECT_EQ(players.getPlayer(2).getRessourceCount(card::Ressource::BRICK), 0);
}

TEST(Monopoly, DoesNot_Affect_NonMonopolyResssource)
{
	Players players;
	players.initializePlayers(2, nullptr);

	card::MonopolyAction action(players);

	players.getPlayer(0).addRessource(card::Ressource::BRICK, 3);
	players.getPlayer(1).addRessource(card::Ressource::BRICK, 5);

	players.getPlayer(0).addRessource(card::Ressource::WOOL, 2);
	players.getPlayer(1).addRessource(card::Ressource::WOOL, 1);

	card::DevelopmentData data;
	data.setMonopolyRessource(card::Ressource::BRICK);

	action.execute(players.getPlayer(0), data);

	EXPECT_EQ(players.getPlayer(0).getRessourceCount(card::Ressource::WOOL), 2);
	EXPECT_EQ(players.getPlayer(1).getRessourceCount(card::Ressource::WOOL), 1);
}

TEST(FreeRessources, AddingTwoDifferentRessources)
{
	player::Player player(0);

	card::FreeRessourcesAction action;
	card::DevelopmentData data;
	data.setFreeRessources({ card::Ressource::BRICK, card::Ressource::LUMBER });

	int initialBrickCount = player.getRessourceCount(card::Ressource::BRICK);
	int initialLumberCount = player.getRessourceCount(card::Ressource::LUMBER);
	int initialWoolCount = player.getRessourceCount(card::Ressource::WOOL);

	action.execute(player, data);

	EXPECT_EQ(player.getRessourceCount(card::Ressource::BRICK) - initialBrickCount, 1);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::LUMBER) - initialLumberCount, 1);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::WOOL) - initialWoolCount, 0);
}

TEST(FreeRessources, AddingSameRessourceTwice)
{
	player::Player player(0);

	card::FreeRessourcesAction action;
	card::DevelopmentData data;
	data.setFreeRessources({ card::Ressource::BRICK, card::Ressource::BRICK });

	int initialBrickCount = player.getRessourceCount(card::Ressource::BRICK);
	int initialLumberCount = player.getRessourceCount(card::Ressource::LUMBER);
	int initialWoolCount = player.getRessourceCount(card::Ressource::WOOL);

	action.execute(player, data);

	EXPECT_EQ(player.getRessourceCount(card::Ressource::BRICK) - initialBrickCount, 2);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::LUMBER) - initialLumberCount, 0);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::WOOL) - initialWoolCount, 0);
}