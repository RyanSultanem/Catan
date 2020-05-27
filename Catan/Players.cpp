#include <Players.hpp>

void Players::initializePlayers(int numberOfPlayers)
{
	m_players.reserve(numberOfPlayers);
	for (int i = 0; i < numberOfPlayers; ++i)
		m_players.push_back(player::Player(i));
}

int Players::getNumberOfPlayers() const
{
	return m_players.size();
}

int Players::getActivePlayerId() const
{
	return m_activePlayer;
}

player::Player & Players::getActivePlayer()
{
	return m_players[m_activePlayer];
}

const std::vector<player::Player> Players::getPlayers() const
{
	return m_players;
}

std::vector<player::Player> Players::getPlayers()
{
	return m_players;
}

void Players::setNextActivePlayer()
{
	m_activePlayer = (m_activePlayer + 1) % m_players.size();
}

void Players::setNextActivePlayer(int playerId)
{
	if (playerId < m_players.size() && playerId >= 0)
		m_activePlayer = playerId;
}
