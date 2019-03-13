#include "Achievement.hpp"

#include "Player.hpp"
#include "Development.hpp"
#include "Edge.h"
#include "Vertex.h"

Achievement::Achievement(int initialCount)
	: m_player(std::nullopt)
	, m_count(initialCount)
{
}

bool Achievement::update(player::Player & player, const AchievementChecker & checker)
{
	bool updated = false;
	int newCount = checker.getAchievementCount(player.getId());

	if(newCount > m_count)
	{
		m_count = newCount;

		if(m_player)
			m_player->get().receivePoints(-2);
		player.receivePoints(2);
		
		m_player = player;
		updated = true;
	}

	return updated;
}

LongestRoadChecker::LongestRoadChecker(const board::Edge & edge)
	: m_edge(edge)
{
}

int getSideLongest(const board::Edge & edge, const board::Vertex & vertex, int playerId)
{
	if (edge.getId() != playerId)
		return 0;

	std::vector<board::EdgeCRef> otherEdges =  vertex.getOtherEdges(edge);
	int edgeCount = otherEdges.size();
	std::vector<int> longestsCount(edgeCount);

	for(int i = 0; i < edgeCount; ++i)
	{
		const board::Edge & otherEdge = otherEdges.at(i);
		board::VertexCRef otherVertex = *otherEdge.getOtherVertex(vertex); // for now assumes that it cannot be wrong.
		
		longestsCount[i] = 1 + getSideLongest(otherEdge, otherVertex, playerId);
	}

	return *std::max_element(longestsCount.begin(), longestsCount.end()); // Also assumes that it cannot be wrong
}

int LongestRoadChecker::getAchievementCount(int playerId) const
{
	if (m_edge.getId() != playerId)
		return 0;

	std::vector<board::VertexCRef> vertices = m_edge.getVertices();
	int firstSideLongest = getSideLongest(m_edge, vertices.at(0), playerId);
	int secondSideLonget = getSideLongest(m_edge, vertices.at(1), playerId);

	return firstSideLongest + secondSideLonget - 1; // TODO: This is wrong when we have loops
}

StrongestArmyChecker::StrongestArmyChecker(const std::vector<card::Development> & developmentsCards)
	: m_developmentsCards(developmentsCards)
{
}

int StrongestArmyChecker::getAchievementCount(int /*playerId*/) const
{
	return std::count_if(m_developmentsCards.begin(), m_developmentsCards.end(),
		[](const card::Development & devCard)
	{
		return devCard.isUsed() && devCard.getType() == card::DevelopmentType::Knight;
	});
}
