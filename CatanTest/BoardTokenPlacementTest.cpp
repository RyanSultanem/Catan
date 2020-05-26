#include "gtest/gtest.h"

#include <board/Board.hpp>
#include <board/factory/BoardFactory.hpp>

#include <token/Conditions.hpp>

#include <player/Player.hpp>

#include <memory>

TEST(BoardTokenInitialPlacement, SettlementAndRoad)
{
	board::Board board = board::BoardFactory().generateBoard();

	player::Player player(0);

	int settlementPosition = 0;
	int roadPosition = 1;

	bool firstSettlementResult = board.placeSettlement(settlementPosition, *player.getSettlement(), PlaceInitialSettlementCondition(player.getId(), roadPosition));
	
	bool firstRoadResult = board.placeRoad(roadPosition, *player.getRoad(), PlaceInitialRoadCondition(player.getId(), settlementPosition));

	EXPECT_TRUE(firstSettlementResult);
	EXPECT_TRUE(firstRoadResult);
}

TEST(BoardTokenInitialPlacement, SettlementAndRoad_NotConnected)
{
	board::Board board = board::BoardFactory().generateBoard();

	player::Player player(0);

	int settlementPosition = 0;
	int roadPosition = 5;

	bool firstSettlementResult = board.placeSettlement(settlementPosition, *player.getSettlement(), PlaceInitialSettlementCondition(player.getId(), roadPosition));

	bool firstRoadResult = board.placeRoad(roadPosition, *player.getRoad(), PlaceInitialRoadCondition(player.getId(), settlementPosition));

	EXPECT_FALSE(firstSettlementResult);
	EXPECT_FALSE(firstRoadResult);
}

struct BoardTokenPlacementTest : public testing::Test
{
	board::Board board;
	std::unique_ptr<player::Player> player; // need of unique_ptr because of issue with reinitialization
											// TODO: check how to do better on stack.

	int initialSettlementPosition;
	int initialRoadPosition;

	void SetUp() override
	{
		initializeVariables();
		setupValidInitialSettlementRoad();
	}

private:
	void initializeVariables()
	{
		board::BoardFactory factory;
		board = factory.generateBoard();

		player = std::make_unique<player::Player>(0);

		initialSettlementPosition = 0;
		initialRoadPosition = 1;
	}

	void setupValidInitialSettlementRoad()
	{
		bool firstSettlementResult = board.placeSettlement(initialSettlementPosition, *player->getSettlement(), PlaceInitialSettlementCondition(player->getId(), initialRoadPosition));

		bool firstRoadResult = board.placeRoad(initialRoadPosition, *player->getRoad(), PlaceInitialRoadCondition(player->getId(), initialSettlementPosition));

		ASSERT_TRUE(firstSettlementResult);
		ASSERT_TRUE(firstRoadResult);
	}
};

TEST_F(BoardTokenPlacementTest, AdjacentRoadValid_NewSettlementAfterRoad)
{
	int newRoadPosition = 2;
	int newSettlementPosition = 1;

	bool newRoadResult = board.placeRoad(newRoadPosition, *player->getRoad(), PlaceRoadCondition(player->getId()));

	bool newSettlementResult = board.placeSettlement(newSettlementPosition, *player->getSettlement(), PlaceSettlementCondition(player->getId()));

	EXPECT_TRUE(newRoadResult);
	EXPECT_TRUE(newSettlementResult);
}


TEST_F(BoardTokenPlacementTest, OnAlreadyConstructed_Fail)
{
	int newRoadPosition = initialRoadPosition;
	int newSettlementPosition = initialSettlementPosition;

	bool newRoadResult = board.placeRoad(newRoadPosition, *player->getRoad(), PlaceRoadCondition(player->getId()));

	bool newSettlementResult = board.placeSettlement(newSettlementPosition, *player->getSettlement(),PlaceSettlementCondition(player->getId()));

	EXPECT_FALSE(newRoadResult);
	EXPECT_FALSE(newSettlementResult);
}

TEST_F(BoardTokenPlacementTest, SettlementOnNextVertex_Fail)
{
	int newSettlementPosition = 4;

	bool newSettlementResult = board.placeSettlement(newSettlementPosition, *player->getSettlement(), PlaceSettlementCondition(player->getId()));

	EXPECT_FALSE(newSettlementResult);
}

TEST_F(BoardTokenPlacementTest, RoadOnOtherSideOfSettlement)
{
	int newRoadPosition = 0;

	bool newRoadResult = board.placeRoad(newRoadPosition, *player->getRoad(), PlaceRoadCondition(player->getId()));

	EXPECT_TRUE(newRoadResult);
}

TEST_F(BoardTokenPlacementTest, RoadNotAdjacent_Fail)
{
	int newRoadPosition = 12;

	bool newRoadResult = board.placeRoad(newRoadPosition, *player->getRoad(), PlaceRoadCondition(player->getId()));

	EXPECT_FALSE(newRoadResult);
}

TEST_F(BoardTokenPlacementTest, AdjacentRoadValid_NewSettlementAfterRoadOtherPlayer_Fail)
{
	int newRoadPosition = 2;

	player::Player player2(1);
	int newSettlementPositionPlayer2 = 1;
		
	board.placeRoad(newRoadPosition, *player->getRoad(), PlaceRoadCondition(player->getId()));

	bool newSettlementResult = board.placeSettlement(newSettlementPositionPlayer2, *player2.getSettlement(), PlaceSettlementCondition(player2.getId()));

	EXPECT_FALSE(newSettlementResult);
}

TEST_F(BoardTokenPlacementTest, CityOnInitialSettlementPosition)
{
	int cityPosition = initialSettlementPosition;

	bool cityResult = board.placeCity(cityPosition, *player->getCity(), PlaceCityCondition(player->getId()));

	EXPECT_TRUE(cityResult);
}

TEST_F(BoardTokenPlacementTest, CityOnAdjacentValidRoadSettlement)
{
	int newRoadPosition = 2;
	int newSettlementPosition = 1;

	int cityPosition = newSettlementPosition;

	board.placeRoad(newRoadPosition, *player->getRoad(), PlaceRoadCondition(player->getId()));

	board.placeSettlement(newSettlementPosition, *player->getSettlement(), PlaceSettlementCondition(player->getId()));

	bool cityResult = board.placeCity(cityPosition, *player->getCity(), PlaceCityCondition(player->getId()));

	EXPECT_TRUE(cityResult);
}

TEST_F(BoardTokenPlacementTest, CityOnValidRoad_ButNoSettlement_Fail)
{
	int newRoadPosition = 2;
	int newSettlementPosition = 1;

	int cityPosition = newSettlementPosition;

	board.placeRoad(newRoadPosition, *player->getRoad(), PlaceRoadCondition(player->getId()));

	bool cityResult = board.placeCity(cityPosition, *player->getCity(), PlaceCityCondition(player->getId()));

	EXPECT_FALSE(cityResult);
}