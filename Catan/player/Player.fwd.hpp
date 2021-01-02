#ifndef PLAYER_PLAYER_FWD_HPP
#define PLAYER_PLAYER_FWD_HPP

#include <functional>

namespace player {

class Player;
typedef std::reference_wrapper<Player> PlayerRef;

class Owner;
typedef std::reference_wrapper<Owner> OwnerRef;
typedef std::reference_wrapper<const Owner> OwnerCRef;

class PointReceiver;
typedef std::reference_wrapper<PointReceiver> PointReceiverRef;

} // namespace player

#endif // !PLAYER_PLAYER_FWD_HPP
