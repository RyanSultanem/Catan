#include "gtest/gtest.h"

#include <board/Board.hpp>
#include <board/factory/BoardFactory.hpp>

#include <card/Card.hpp>

using namespace board;

struct BoardInitializationTest : public testing::Test
{
	static Board board;
	static void SetUpTestCase()
	{
		BoardFactory factory;
		board = factory.generateBoard();
	}
}; 

Board BoardInitializationTest::board;

TEST_F(BoardInitializationTest, creationTest)
{
	EXPECT_EQ(19, board.getNumberOfCells());
	EXPECT_EQ(54, board.getNumberOfVertices());
	EXPECT_EQ(72, board.getNumberOfEdges());
}

TEST_F(BoardInitializationTest, cellIdTest)
{
	EXPECT_EQ(5, board.getCellId(5));
	EXPECT_EQ(18, board.getCellId(18));
}

TEST_F(BoardInitializationTest, ressourceType)
{
	EXPECT_EQ(3, board.getNumberOfLand(card::RessourceType::BRICK));
	EXPECT_EQ(4, board.getNumberOfLand(card::RessourceType::GRAIN));
	EXPECT_EQ(4, board.getNumberOfLand(card::RessourceType::LUMBER));
	EXPECT_EQ(1, board.getNumberOfLand(card::RessourceType::NO_RESSOURCE));
	EXPECT_EQ(3, board.getNumberOfLand(card::RessourceType::ORE));
	EXPECT_EQ(4, board.getNumberOfLand(card::RessourceType::WOOL));
}

TEST_F(BoardInitializationTest, edgeNeighBoorTest)
{
	EXPECT_EQ(true, board.isConnectedEdges({}));
	EXPECT_EQ(true, board.isConnectedEdges({0, 1, 0}));
	EXPECT_EQ(true, board.isConnectedEdges({ 0, 1, 2 }));
	EXPECT_EQ(true, board.isConnectedEdges({ 35, 27, 28, 29}));
	EXPECT_EQ(true, board.isConnectedEdges({ 29, 28, 27, 35 }));
	EXPECT_EQ(false, board.isConnectedEdges({ 1, 3, 10 }));

	EXPECT_EQ(true, board.isConnectedEdges({ 23, 33, 39, 40 }));
	EXPECT_EQ(false, board.isConnectedEdges({ 0, 1, 7, 19, 24 }));
	EXPECT_EQ(true, board.isConnectedEdges({ 71, 65, 61, 53, 47, 37, 30, 29, 36, 44, 43, 35, 27, 20, 13, 12, 11, 6 }));

	EXPECT_EQ(false, board.isConnectedEdges({ 66, 55, 56 }));
	EXPECT_EQ(false, board.isConnectedEdges({ 66, 62, 56 }));
	EXPECT_EQ(true, board.isConnectedEdges({ 66, 62, 55, 56 }));
}
