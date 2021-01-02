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
	int getLastPlayerId() const;
	player::Player & getActivePlayer();

	player::Player & getPlayer(int playerId);
	const player::Player & getPlayer(int playerId) const;

	const std::vector<player::Player> & getPlayers() const;

	void setNextActivePlayer();
	void setNextActivePlayer(int playerId);

	template<typename Function>
	void forEachPlayer(Function func);

private:
	std::vector<player::Player> m_players;
	int m_activePlayer;
};


template<typename Function>
void Players::forEachPlayer(Function func)
{
	std::for_each(m_players.begin(), m_players.end(),
		[&func](player::Player & player)
		{
			func(player);
		});
}

#endif // !PLAYERS_HPP