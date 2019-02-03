#ifndef PLAYER_REACTIONS_HPP
#define PLAYER_REACTIONS_HPP

#include "Player.fwd.hpp"

namespace player {
namespace reactions {

	//TODO: refactor to use Building*
bool settlementRessourcesAvailable(const Player & player);
bool settlementPay(Player & player);
void settlmentPlaced(Player & player);

bool roadRessourcesAvailable(const Player & player);
bool roadPay(Player & player);
void roadPlaced(Player & player);

bool cityRessourcesAvailable(const Player & player);
bool cityPay(Player & player);
void cityPlaced(Player & player);

} // namespace reactions
} // namespace player

#endif // !PLAYER_REACTIONS_HPP