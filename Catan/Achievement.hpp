#ifndef ACHIEVEMENT_HPP
#define ACHIEVEMENT_HPP

#include "Achievement.fwd.hpp"
#include "Edge.fwd.h"
#include "Player.fwd.hpp"
#include <optional>
#include "Development.fwd.hpp"

class Achievement
{
public:
	explicit Achievement(int initialCount);
	bool update(player::Player & player, const AchievementChecker & checker);

private:
	std::optional<player::PlayerRef> m_player; // TODO: Change to PointReceiver
	int m_count;
};

class AchievementChecker
{
public:
	virtual int getAchievementCount(int playerId) const = 0;
};

class LongestRoadChecker : public AchievementChecker
{
public:
	explicit LongestRoadChecker(const board::Edge & edge);

	int getAchievementCount(int playerId) const override;

private:
	const board::Edge & m_edge;
};

class StrongestArmyChecker : public AchievementChecker
{
public:
	explicit StrongestArmyChecker(const std::vector<card::Development> & developmentsCards);

	int getAchievementCount(int playerId) const override;

private:
	const std::vector<card::Development> & m_developmentsCards;
};

#endif // !ACHIEVEMENT_HPP