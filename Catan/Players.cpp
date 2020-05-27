#include <Players.hpp>

#include <player/Player.hpp>

void Players::initializePlayers(int numberOfPlayers)
{
	m_players.reserve(numberOfPlayers);
	for (int i = 0; i < numberOfPlayers; ++i)
		m_players.push_back(player::Player(i));
}

int Players::getActivePlayerId() const
{
	return m_activePlayer;
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
