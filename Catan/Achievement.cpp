#include <Achievement.hpp>

#include <card/Development.hpp>

#include <board/Edge.hpp>
#include <board/Vertex.hpp>

#include <player/Player.hpp>

#include <set>

Achievement::Achievement(int minRequirement)
	: m_minRequirement(minRequirement)
	, m_pointReceiver(std::nullopt)
	, m_count(0)
{
}

bool Achievement::update(player::PointReceiver & newPointReceiver, const AchievementChecker & checker)
{
	int newCount = checker.getAchievementCount(newPointReceiver.getId());

	if(newCount >= m_minRequirement && newCount > m_count)
	{
		m_count = newCount;
		return updatePlayerInformation(newPointReceiver);
	}

	return false;
}

bool Achievement::updatePlayerInformation(player::PointReceiver & newPointReceiver)
{
	if (m_pointReceiver.has_value() && m_pointReceiver->get().getId() == newPointReceiver.getId())
		return false;
	
	if (m_pointReceiver.has_value())
		m_pointReceiver->get().receivePoints(-2);

	newPointReceiver.receivePoints(2);
	m_pointReceiver = newPointReceiver;

	return true;
}

LongestRoadChecker::LongestRoadChecker(const board::Edge & edge)
	: m_edge(edge)
{
}

static bool visited(const std::set<int> & visistedEdgesIds, int edgeId)
{
	return visistedEdgesIds.find(edgeId) != visistedEdgesIds.end();
}

struct PathLengthVisited
{
	PathLengthVisited(const std::set<int> & visitedEdges, int length)
		: visitedEdgesCopy(visitedEdges)
		, length(length)
	{}

	bool operator<(const PathLengthVisited & first) const
	{
		return this->length < first.length;
	}

	std::set<int> visitedEdgesCopy;
	int length = 0;
};

static int getSideLongest(const board::Edge & edge, const board::Vertex & vertex, int playerId, std::set<int> & visitedEdgesIds)
{
	if (visited(visitedEdgesIds, edge.getId()) || !edge.hasRoadOfPlayer(playerId))
		return 0;
	visitedEdgesIds.insert(edge.getId());

	std::vector<board::EdgeCRef> otherEdges = vertex.getOtherEdges(edge);
	int edgeCount = otherEdges.size();
	std::vector<PathLengthVisited> longestsCountVisited(edgeCount, PathLengthVisited(visitedEdgesIds, 0)); // Copies of Sets is important for cases with loops

	for(int i = 0; i < edgeCount; ++i)
	{
		const board::Edge & otherEdge = otherEdges.at(i);
		board::VertexCRef otherVertex = *otherEdge.getOtherVertex(vertex);
		
		longestsCountVisited[i].length = 1 + getSideLongest(otherEdge, otherVertex, playerId, longestsCountVisited[i].visitedEdgesCopy);
	}

	const PathLengthVisited & subLongest = *std::max_element(longestsCountVisited.begin(), longestsCountVisited.end());
	visitedEdgesIds = subLongest.visitedEdgesCopy;

	return subLongest.length;
}

int LongestRoadChecker::getAchievementCount(int playerId) const
{
	if (!m_edge.hasRoadOfPlayer(playerId))
		return 0;

	// TODO: To resolve loop issues, Add a part to find the edges that are at extremities
	// Then compute for each of these edges
	// Result is the max of those edges.
	// If none is found, simply continue with the given edge, it means all the edges are in a loop, and it wont matter.

	std::vector<board::VertexCRef> vertices = m_edge.getVertices();
	std::set<int> visitedEdgesIds;

	int firstSideLongest = getSideLongest(m_edge, vertices.at(0), playerId, visitedEdgesIds);
	visitedEdgesIds.erase(m_edge.getId());
	int secondSideLonget = getSideLongest(m_edge, vertices.at(1), playerId, visitedEdgesIds);

	return firstSideLongest + secondSideLonget - 1; // TODO: This is wrong when we have loops
}

StrongestArmyChecker::StrongestArmyChecker(const player::Player & player)
	: m_player(player)
{
}

int StrongestArmyChecker::getAchievementCount(int playerId) const
{
	if(playerId != m_player.getId())
		return 0;

	std::vector<card::DevelopmentCRef> usedDevelopment = m_player.getUsedDevelopments();

	return std::count_if(usedDevelopment.begin(), usedDevelopment.end(),
		[](const card::Development & devCard)
	{
		return devCard.isUsed() && devCard.getType() == card::DevelopmentType::Knight;
	});
}
