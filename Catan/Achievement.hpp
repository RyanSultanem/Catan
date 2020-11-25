#ifndef ACHIEVEMENT_HPP
#define ACHIEVEMENT_HPP

#include <Achievement.fwd.hpp>

#include <board/Edge.fwd.hpp>

#include <card/Development.fwd.hpp>

#include <player/Player.fwd.hpp>

#include <optional>

class Achievement
{
public:
	explicit Achievement(int minRequirement);
	bool update(player::PointReceiver & newPointReceiver, const AchievementChecker & checker);

private:
	const int m_minRequirement;

	std::optional<player::PointReceiverRef> m_pointReceiver;
	int m_count;

private:
	bool updatePlayerInformation(player::PointReceiver & newPointReceiver);
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
	explicit StrongestArmyChecker(const player::Player & player);

	int getAchievementCount(int playerId) const override;

private:
	const player::Player & m_player;
};

#endif // !ACHIEVEMENT_HPP