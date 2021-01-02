#include <Achievement.hpp>

#include <card/Development.hpp>
#include <board/Board.hpp>
#include <board/factory/BoardFactory.hpp>

#include <Players.hpp>
#include <player/Player.hpp>

#include <mock/PlaceConditionMock.hpp>

#include <utility/NumberGenerator.hpp>

#include <array>

#include <gtest/gtest.h>

class AchievementCheckerMock : public AchievementChecker
{
public:
	AchievementCheckerMock(const std::vector<int> & values)
		: m_values(values)
	{
	}

	int getAchievementCount(int /*playerId*/) const override
	{
		int value = m_values[currentIndex % m_values.size()];
		++*(const_cast<int*>(&currentIndex)); // Ok for a mock
		return value;
	}

private:
	std::vector<int> m_values;
	int currentIndex = 0;
};

TEST(AchievementGlobalTest, LimitAndUpdateTest)
{
	Achievement achievement(3);
	player::Player player(0);
	int initialPlayerPoints = player.getPoints();

	AchievementCheckerMock checkerMock({ 2,3,5 });
	{
		SCOPED_TRACE(0);
		bool result = achievement.update(player, checkerMock);
		EXPECT_EQ(player.getPoints() - initialPlayerPoints, 0);
		EXPECT_EQ(result, false);
	}

	{
		SCOPED_TRACE(1);
		bool result = achievement.update(player, checkerMock);
		EXPECT_EQ(player.getPoints() - initialPlayerPoints, 2);
		EXPECT_EQ(result, true);
	}

	{
		SCOPED_TRACE(2);
		bool result = achievement.update(player, checkerMock);
		EXPECT_EQ(player.getPoints() - initialPlayerPoints, 2);
		EXPECT_EQ(result, false);
	}
}

TEST(AchievementGlobalTest, OwnerChange)
{
	Achievement achievement(3);
	player::Player player0(0);
	int player0InitialPts = player0.getPoints();
	player::Player player1(1);
	int player1InitialPts = player1.getPoints();

	AchievementCheckerMock checkerMock({ 3,4 });
	{
		SCOPED_TRACE(0);
		bool result = achievement.update(player0, checkerMock);
		EXPECT_EQ(player0.getPoints() - player0InitialPts, 2);
		EXPECT_EQ(player1.getPoints() - player1InitialPts, 0);
		EXPECT_EQ(result, true);
	}

	{
		SCOPED_TRACE(1);
		bool result = achievement.update(player1, checkerMock);
		EXPECT_EQ(player0.getPoints() - player0InitialPts, 0);
		EXPECT_EQ(player1.getPoints() - player1InitialPts, 2);
		EXPECT_EQ(result, true);
	}
}

TEST(AchievementGlobalTest, NoOwnerChangeOnEqualUpdate)
{
	Achievement achievement(3);
	player::Player player0(0);
	int player0InitialPts = player0.getPoints();
	player::Player player1(1);
	int player1InitialPts = player1.getPoints();

	AchievementCheckerMock checkerMock({ 5,5 });
	achievement.update(player0, checkerMock);
	achievement.update(player1, checkerMock);

	bool result = achievement.update(player0, checkerMock);
	EXPECT_EQ(player0.getPoints() - player0InitialPts, 2);
	EXPECT_EQ(player1.getPoints() - player1InitialPts, 0);
	EXPECT_EQ(result, false);	
}

static void simulateUseDevelopment(player::Player & player, const card::DevelopmentAction & developmentAction, int count, int countUsed)
{
	if (countUsed > count)
		return;

	for (int i = 0; i < count; ++i)
		player.receiveDevelopment(card::Development(developmentAction));

	card::DevelopmentData data;
	std::array<std::pair<int, int>, 2> cellVertexPositons;
	cellVertexPositons[0] = { 5,10 }; // random different valid cell/vertex values for correct robber movement
	cellVertexPositons[1] = { 6,20 };

	for (int i = 0; i < countUsed; ++i)
	{
		std::pair<int, int> cellVertexPosition = cellVertexPositons[i%cellVertexPositons.size()];
		data.setCellVertexPosition({ cellVertexPosition.first, cellVertexPosition.second });
		auto & developmentOpt = player.getUnusedDevelopment(developmentAction.getType());
		if (developmentOpt)
			developmentOpt->get().executeAction(player, data);
	}
}

TEST(StrongestArmyTest, KnightCountAndUsedEqual)
{
	Players players;
	players.initializePlayers(1, nullptr);
	player::Player & player = players.getPlayer(0);
	
	int knightCount = 3;
	int knightCountUsed = 3;

	// TODO: Required here for scope.. should be able to do better when development reduces dependency
	board::Board board = board::BoardFactory().generateBoard();
	Achievement strongestArmy(3);
	card::KnightAction knightAction(players, board, strongestArmy, RandomNumberGenerator());
	simulateUseDevelopment(player, knightAction, knightCount, knightCountUsed);

	StrongestArmyChecker checker(player);
	int result = checker.getAchievementCount(player.getId());

	EXPECT_EQ(result, knightCountUsed);
}

TEST(StrongestArmyTest, KnightCountGreaterThanUsed)
{
	Players players;
	players.initializePlayers(1, nullptr);
	player::Player & player = players.getPlayer(0);	
	
	int knightCount = 5;
	int knightCountUsed = 2;

	// TODO: Required here for scope.. should be able to do better
	board::Board board = board::BoardFactory().generateBoard();
	Achievement strongestArmy(3);
	card::KnightAction knightAction(players, board, strongestArmy, RandomNumberGenerator());
	simulateUseDevelopment(player, knightAction, knightCount, knightCountUsed);

	StrongestArmyChecker checker(player);
	int result = checker.getAchievementCount(player.getId());

	EXPECT_EQ(result, knightCountUsed);
}

TEST(StrongestArmyTest, KnightCountUsedWithOtherDevelopmentInPlay)
{
	Players players;
	players.initializePlayers(1, nullptr);
	player::Player & player = players.getPlayer(0);	
	
	int knightCount = 5;
	int knightCountUsed = 2;

	// TODO: Required here for scope.. should be able to do better
	board::Board board = board::BoardFactory().generateBoard();
	Achievement strongestArmy(3);
	card::KnightAction knightAction(players, board, strongestArmy, RandomNumberGenerator());
	simulateUseDevelopment(player, knightAction, knightCount, knightCountUsed);
	card::MonopolyAction monopolyAction(players);
	int monopolyCount = 2;
	int monopolyCountUsed = 1;
	simulateUseDevelopment(player, monopolyAction, monopolyCount, monopolyCountUsed);

	StrongestArmyChecker checker(player);
	int result = checker.getAchievementCount(player.getId());

	EXPECT_EQ(result, knightCountUsed);
}

TEST(LongestRoadTest, FirstEdgeTest)
{
	player::Player player(0);
	board::Board board = board::BoardFactory().generateBoard();
	PlaceRoadConditionMock trueConditionMocked(player.getId());

	board.placeRoad(0, *player.getRoad(), trueConditionMocked);
	board.placeRoad(1, *player.getRoad(), trueConditionMocked);
	board.placeRoad(2, *player.getRoad(), trueConditionMocked);

	std::optional<board::EdgeCRef> edgeOpt = board.getEdgeAt(0);

	LongestRoadChecker checker(*edgeOpt);
	int firstLength = checker.getAchievementCount(player.getId());
	EXPECT_EQ(firstLength, 3);

	board.placeRoad(3, *player.getRoad(), trueConditionMocked);
	int secondLength = checker.getAchievementCount(player.getId());
	EXPECT_EQ(secondLength, 4);
}

TEST(LongestRoadTest, LastEdgeTest)
{
	player::Player player(0);
	board::Board board = board::BoardFactory().generateBoard();
	PlaceRoadConditionMock trueConditionMocked(player.getId());

	board.placeRoad(0, *player.getRoad(), trueConditionMocked);
	board.placeRoad(1, *player.getRoad(), trueConditionMocked);
	board.placeRoad(2, *player.getRoad(), trueConditionMocked);

	std::optional<board::EdgeCRef> edgeOpt = board.getEdgeAt(2);

	LongestRoadChecker checker(*edgeOpt);
	int length = checker.getAchievementCount(player.getId());
	EXPECT_EQ(length, 3);
}

TEST(LongestRoadTest, MiddleEdgeTest)
{
	player::Player player(0);
	board::Board board = board::BoardFactory().generateBoard();
	PlaceRoadConditionMock trueConditionMocked(player.getId());

	board.placeRoad(0, *player.getRoad(), trueConditionMocked);
	board.placeRoad(1, *player.getRoad(), trueConditionMocked);
	board.placeRoad(2, *player.getRoad(), trueConditionMocked);

	std::optional<board::EdgeCRef> edgeOpt = board.getEdgeAt(1);

	LongestRoadChecker checker(*edgeOpt);
	int length = checker.getAchievementCount(player.getId());
	EXPECT_EQ(length, 3);
}

TEST(LongestRoadTest, LongestRoadWithShorterRamifications)
{
	player::Player player(0);
	board::Board board = board::BoardFactory().generateBoard();
	PlaceRoadConditionMock trueConditionMocked(player.getId());

	board.placeRoad(12, *player.getRoad(), trueConditionMocked);
	board.placeRoad(13, *player.getRoad(), trueConditionMocked);
	board.placeRoad(14, *player.getRoad(), trueConditionMocked);
	board.placeRoad(20, *player.getRoad(), trueConditionMocked);
	board.placeRoad(28, *player.getRoad(), trueConditionMocked);
	board.placeRoad(29, *player.getRoad(), trueConditionMocked);
	board.placeRoad(27, *player.getRoad(), trueConditionMocked);

	std::optional<board::EdgeCRef> edgeOpt = board.getEdgeAt(12);

	LongestRoadChecker checker(*edgeOpt);
	int length = checker.getAchievementCount(player.getId());
	EXPECT_EQ(length, 5);
}

TEST(LongestRoadTest, LoopOnly)
{
	player::Player player(0);
	board::Board board = board::BoardFactory().generateBoard();
	PlaceRoadConditionMock trueConditionMocked(player.getId());

	board.placeRoad(12, *player.getRoad(), trueConditionMocked);
	board.placeRoad(13, *player.getRoad(), trueConditionMocked);
	board.placeRoad(20, *player.getRoad(), trueConditionMocked);
	board.placeRoad(27, *player.getRoad(), trueConditionMocked);
	board.placeRoad(26, *player.getRoad(), trueConditionMocked);
	board.placeRoad(19, *player.getRoad(), trueConditionMocked);

	std::optional<board::EdgeCRef> edgeOpt = board.getEdgeAt(12);

	LongestRoadChecker checker(*edgeOpt);
	int length = checker.getAchievementCount(player.getId());
	EXPECT_EQ(length, 6);
}

TEST(LongestRoadTest, LoopWithSingleRamification_FromRamification)
{
	player::Player player(0);
	board::Board board = board::BoardFactory().generateBoard();
	PlaceRoadConditionMock trueConditionMocked(player.getId());

	//Loop
	board.placeRoad(12, *player.getRoad(), trueConditionMocked);
	board.placeRoad(13, *player.getRoad(), trueConditionMocked);
	board.placeRoad(20, *player.getRoad(), trueConditionMocked);
	board.placeRoad(27, *player.getRoad(), trueConditionMocked);
	board.placeRoad(26, *player.getRoad(), trueConditionMocked);
	board.placeRoad(19, *player.getRoad(), trueConditionMocked);

	//Ramification Size 1
	board.placeRoad(25, *player.getRoad(), trueConditionMocked);

	std::optional<board::EdgeCRef> edgeOpt = board.getEdgeAt(25);

	LongestRoadChecker checker(*edgeOpt);
	int length = checker.getAchievementCount(player.getId());
	EXPECT_EQ(length, 7); // Size of loop + Ramification Size
}

TEST(LongestRoadTest, DISABLED_LoopWithSingleRamification_FromLoop)
{
	player::Player player(0);
	board::Board board = board::BoardFactory().generateBoard();
	PlaceRoadConditionMock trueConditionMocked(player.getId());

	//Loop
	board.placeRoad(12, *player.getRoad(), trueConditionMocked);
	board.placeRoad(13, *player.getRoad(), trueConditionMocked);
	board.placeRoad(20, *player.getRoad(), trueConditionMocked);
	board.placeRoad(27, *player.getRoad(), trueConditionMocked);
	board.placeRoad(26, *player.getRoad(), trueConditionMocked);
	board.placeRoad(19, *player.getRoad(), trueConditionMocked);

	//Ramification Size 1
	board.placeRoad(25, *player.getRoad(), trueConditionMocked);

	std::optional<board::EdgeCRef> edgeOpt = board.getEdgeAt(12);

	LongestRoadChecker checker(*edgeOpt);
	int length = checker.getAchievementCount(player.getId());
	EXPECT_EQ(length, 7); // Size of loop + Ramification Size
}

TEST(LongestRoadTest, DISABLED_LoopWithDifferentRamificationSize_FromLoop)
{
	player::Player player(0);
	board::Board board = board::BoardFactory().generateBoard();
	PlaceRoadConditionMock trueConditionMocked(player.getId());

	//Loop
	board.placeRoad(12, *player.getRoad(), trueConditionMocked);
	board.placeRoad(13, *player.getRoad(), trueConditionMocked);
	board.placeRoad(20, *player.getRoad(), trueConditionMocked);
	board.placeRoad(27, *player.getRoad(), trueConditionMocked);
	board.placeRoad(26, *player.getRoad(), trueConditionMocked);
	board.placeRoad(19, *player.getRoad(), trueConditionMocked);

	//Ramification Size 1
	board.placeRoad(25, *player.getRoad(), trueConditionMocked);

	//Ramification Size 2
	board.placeRoad(28, *player.getRoad(), trueConditionMocked);
	board.placeRoad(29, *player.getRoad(), trueConditionMocked);

	std::optional<board::EdgeCRef> edgeOpt = board.getEdgeAt(12);

	LongestRoadChecker checker(*edgeOpt);
	int length = checker.getAchievementCount(player.getId());
	EXPECT_EQ(length, 8); // Size of loop + Biggest Ramification
}

TEST(LongestRoadTest, LoopWithDifferentRamificationSize_FromBigRamification)
{
	player::Player player(0);
	board::Board board = board::BoardFactory().generateBoard();
	PlaceRoadConditionMock trueConditionMocked(player.getId());

	//Loop
	board.placeRoad(12, *player.getRoad(), trueConditionMocked);
	board.placeRoad(13, *player.getRoad(), trueConditionMocked);
	board.placeRoad(20, *player.getRoad(), trueConditionMocked);
	board.placeRoad(27, *player.getRoad(), trueConditionMocked);
	board.placeRoad(26, *player.getRoad(), trueConditionMocked);
	board.placeRoad(19, *player.getRoad(), trueConditionMocked);

	//Ramification Size 1
	board.placeRoad(25, *player.getRoad(), trueConditionMocked);

	//Ramification Size 2
	board.placeRoad(28, *player.getRoad(), trueConditionMocked);
	board.placeRoad(29, *player.getRoad(), trueConditionMocked);

	std::optional<board::EdgeCRef> edgeOpt = board.getEdgeAt(29);

	LongestRoadChecker checker(*edgeOpt);
	int length = checker.getAchievementCount(player.getId());
	EXPECT_EQ(length, 8); // Size of loop + Biggest Ramification
}

TEST(LongestRoadTest, DISABLED_LoopWithDifferentRamificationSize_FromSmallRamification)
{
	player::Player player(0);
	board::Board board = board::BoardFactory().generateBoard();
	PlaceRoadConditionMock trueConditionMocked(player.getId());

	//Loop
	board.placeRoad(12, *player.getRoad(), trueConditionMocked);
	board.placeRoad(13, *player.getRoad(), trueConditionMocked);
	board.placeRoad(20, *player.getRoad(), trueConditionMocked);
	board.placeRoad(27, *player.getRoad(), trueConditionMocked);
	board.placeRoad(26, *player.getRoad(), trueConditionMocked);
	board.placeRoad(19, *player.getRoad(), trueConditionMocked);

	//Ramification Size 1
	board.placeRoad(25, *player.getRoad(), trueConditionMocked);

	//Ramification Size 2
	board.placeRoad(28, *player.getRoad(), trueConditionMocked);
	board.placeRoad(29, *player.getRoad(), trueConditionMocked);

	std::optional<board::EdgeCRef> edgeOpt = board.getEdgeAt(25);

	LongestRoadChecker checker(*edgeOpt);
	int length = checker.getAchievementCount(player.getId());
	EXPECT_EQ(length, 8); // Size of loop + Biggest Ramification
}

TEST(LongestRoadTest, LoopWithTwoNeighborRamifications_FromRamification)
{
	player::Player player(0);
	board::Board board = board::BoardFactory().generateBoard();
	PlaceRoadConditionMock trueConditionMocked(player.getId());

	//Loop
	board.placeRoad(12, *player.getRoad(), trueConditionMocked);
	board.placeRoad(13, *player.getRoad(), trueConditionMocked);
	board.placeRoad(20, *player.getRoad(), trueConditionMocked);
	board.placeRoad(27, *player.getRoad(), trueConditionMocked);
	board.placeRoad(26, *player.getRoad(), trueConditionMocked);
	board.placeRoad(19, *player.getRoad(), trueConditionMocked);

	//Ramification Size 2
	board.placeRoad(28, *player.getRoad(), trueConditionMocked);
	board.placeRoad(29, *player.getRoad(), trueConditionMocked);

	//Ramification Size 2
	board.placeRoad(14, *player.getRoad(), trueConditionMocked);
	board.placeRoad(15, *player.getRoad(), trueConditionMocked);

	std::optional<board::EdgeCRef> edgeOpt = board.getEdgeAt(28);

	LongestRoadChecker checker(*edgeOpt);
	int length = checker.getAchievementCount(player.getId());
	EXPECT_EQ(length, 9); // Size of loop + both ramifications - 1(from loop)
}

TEST(LongestRoadTest, DISABLED_LoopWithTwoNeighborRamifications_FromLoop)
{
	player::Player player(0);
	board::Board board = board::BoardFactory().generateBoard();
	PlaceRoadConditionMock trueConditionMocked(player.getId());

	//Loop
	board.placeRoad(12, *player.getRoad(), trueConditionMocked);
	board.placeRoad(13, *player.getRoad(), trueConditionMocked);
	board.placeRoad(20, *player.getRoad(), trueConditionMocked);
	board.placeRoad(27, *player.getRoad(), trueConditionMocked);
	board.placeRoad(26, *player.getRoad(), trueConditionMocked);
	board.placeRoad(19, *player.getRoad(), trueConditionMocked);

	//Ramification Size 2
	board.placeRoad(28, *player.getRoad(), trueConditionMocked);
	board.placeRoad(29, *player.getRoad(), trueConditionMocked);

	//Ramification Size 2
	board.placeRoad(14, *player.getRoad(), trueConditionMocked);
	board.placeRoad(15, *player.getRoad(), trueConditionMocked);

	std::optional<board::EdgeCRef> edgeOpt = board.getEdgeAt(12);

	LongestRoadChecker checker(*edgeOpt);
	int length = checker.getAchievementCount(player.getId());
	EXPECT_EQ(length, 9); // Size of loop + both ramifications - 1(from loop)
}