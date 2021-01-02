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

int Players::getLastPlayerId() const
{
	return m_players.back().getId();
}

player::Player & Players::getActivePlayer()
{
	return m_players[m_activePlayer];
}

player::Player & Players::getPlayer(int playerId)
{
	return m_players[playerId];
}

const player::Player & Players::getPlayer(int playerId) const
{
	return getPlayer(playerId);
}

const std::vector<player::Player> & Players::getPlayers() const
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
