#include <card/Development.hpp>

#include <Achievement.hpp>

#include <board/Board.hpp>
#include <board/factory/BoardFactory.hpp>

#include <mock/PlaceConditionMock.hpp>

#include <player/Player.hpp>

#include <gtest/gtest.h>

TEST(DevelopmentTest, BuildTwoFreeRoads_SimpleWorking)
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

TEST(DevelopmentTest, BuildTwoFreeRoads_SimpleWorking_InverseOrder)
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

TEST(DevelopmentTest, BuildTwoFreeRoads_OneOnAlreadyPlaced)
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

TEST(DevelopmentTest, BuildTwoFreeRoads_OneOnAlreadyPlaced_Inverse)
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

TEST(DevelopmentTest, BuildTwoFreeRoads_BothOnAlreadyPlaced)
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

TEST(DevelopmentTest, BuildTwoFreeRoads_NotConnectedToInitialRoad)
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