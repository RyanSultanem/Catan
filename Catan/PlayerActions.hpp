#ifndef PLAYER_ACTIONS_HPP
#define PLAYER_ACTIONS_HPP

#include "Player.fwd.hpp"
#include "Building.fwd.hpp"

namespace player {
namespace actions {

void playerSettlmentPlacementReaction(Player & player, const token::building::Settlement & settlement);

} // namespace actions
} // namespace player

#endif // !PLAYER_ACTIONS_HPP