#include <Achievement.hpp>

#include <card/Development.hpp>
#include <board/Board.hpp>
#include <board/factory/BoardFactory.hpp>
#include <player/Player.hpp>

#include <utility/NumberGenerator.hpp>

#include <gtest/gtest.h>


// TODO: place in a seperate mock file.. myabe too small till now
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
	data.setCellVertexPosition({5, 5}); // random values
	for (int i = 0; i < countUsed; ++i)
	{
		auto & developmentOpt = player.getUnusedDevelopment(developmentAction.getType());
		if (developmentOpt)
			developmentOpt->get().executeAction(player, data);
	}
}

TEST(StrongestArmyTest, KnightCountAndUsedEqual)
{
	player::Player player(0);
	int knightCount = 3;
	int knightCountUsed = 3;

	// TODO: Required here for scope.. should be able to do better when development reduces dependency
	board::Board board = board::BoardFactory().generateBoard();
	std::vector<player::Player> players{ player };
	card::KnightAction knightAction(players, board, RandomNumberGenerator());
	simulateUseDevelopment(player, knightAction, knightCount, knightCountUsed);

	StrongestArmyChecker checker(player);
	int result = checker.getAchievementCount(player.getId());

	EXPECT_EQ(result, knightCountUsed);
}

TEST(StrongestArmyTest, KnightCountGreaterThanUsed)
{
	player::Player player(0);
	int knightCount = 5;
	int knightCountUsed = 2;

	// TODO: Required here for scope.. should be able to do better
	board::Board board = board::BoardFactory().generateBoard();
	std::vector<player::Player> players{ player };
	card::KnightAction knightAction(players, board, RandomNumberGenerator());
	simulateUseDevelopment(player, knightAction, knightCount, knightCountUsed);

	StrongestArmyChecker checker(player);
	int result = checker.getAchievementCount(player.getId());

	EXPECT_EQ(result, knightCountUsed);
}

TEST(StrongestArmyTest, KnightCountUsedWithOtherDevelopmentInPlay)
{
	player::Player player(0);
	int knightCount = 5;
	int knightCountUsed = 2;

	// TODO: Required here for scope.. should be able to do better
	board::Board board = board::BoardFactory().generateBoard();
	std::vector<player::Player> players{ player };
	card::KnightAction knightAction(players, board, RandomNumberGenerator());
	simulateUseDevelopment(player, knightAction, knightCount, knightCountUsed);
	card::MonopolyAction monopolyAction(players);
	int monopolyCount = 2;
	int monopolyCountUsed = 1;
	simulateUseDevelopment(player, monopolyAction, monopolyCount, monopolyCountUsed);

	StrongestArmyChecker checker(player);
	int result = checker.getAchievementCount(player.getId());

	EXPECT_EQ(result, knightCountUsed);
}