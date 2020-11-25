#ifndef PLAYER_PLAYER_FWD_HPP
#define PLAYER_PLAYER_FWD_HPP

#include <functional>

namespace player {

class Player;
typedef std::reference_wrapper<Player> PlayerRef;

class PointReceiver;
typedef std::reference_wrapper<PointReceiver> PointReceiverRef;

} // namespace player

#endif // !PLAYER_PLAYER_FWD_HPP
