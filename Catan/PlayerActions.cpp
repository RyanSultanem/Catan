#include "PlayerActions.hpp"

#include "Player.hpp"
#include "Building.hpp"

namespace player {
namespace actions {

void playerSettlmentPlacementReaction(Player & player, const token::building::Settlement & settlement)
{
   player.decreaseSettlmentCount();
   player.receivePoints(settlement.getPoints());
}

} // namespace actions
} // namespace player

