#ifndef PLAYERS_HPP
#define PLAYERS_HPP

#include <player/Player.fwd.hpp>

#include <vector>

class Players
{
public:

	void initializePlayers(int numberOfPlayers);

	int getNumberOfPlayers() const;
	int getActivePlayerId() const;
	player::Player & getActivePlayer();
	const std::vector<player::Player> getPlayers() const;
	std::vector<player::Player> getPlayers();

	void setNextActivePlayer();
	void setNextActivePlayer(int playerId);

private:
	std::vector<player::Player> m_players;
	int m_activePlayer;
};

#endif // !PLAYERS_HPP