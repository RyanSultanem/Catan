#ifndef PLAYER_REACTIONS_HPP
#define PLAYER_REACTIONS_HPP

#include "Player.fwd.hpp"
#include "Building.fwd.hpp"

namespace player {
namespace reactions {

void settlmentPlacement(Player & player, const token::building::Settlement & settlement);

} // namespace reactions
} // namespace player

#endif // !PLAYER_REACTIONS_HPP