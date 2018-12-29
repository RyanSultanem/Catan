#include "PlayerReactions.hpp"

#include "Player.hpp"
#include "Building.hpp"

namespace player {
namespace reactions {

void settlmentPlacement(Player & player, const token::building::Settlement & settlement)
{
   player.decreaseSettlmentCount();
   player.receivePoints(settlement.getPoints());
}

} // namespace reactions
} // namespace player

