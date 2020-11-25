#ifndef PLAYERS_HPP
#define PLAYERS_HPP

#include <GameEndingListenner.hpp>

#include <player/Player.hpp>

#include <vector>

class Players
{
public:
	void initializePlayers(int numberOfPlayers, GameEndingListenner * gameEndListenner);

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