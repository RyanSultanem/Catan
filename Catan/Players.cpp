#include <Players.hpp>

#include <utility/Utility.hpp>

void Players::initializePlayers(int numberOfPlayers, GameEndingListenner * gameEndListenner)
{
	m_players.reserve(numberOfPlayers);
	for (int i = 0; i < numberOfPlayers; ++i)
	{
		m_players.push_back(player::Player(i));
		m_players[i].addGameEndListenner(gameEndListenner);
	}
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
	if (utility::isValidPosition(m_players, playerId))
		m_activePlayer = playerId;
}
