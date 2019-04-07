#ifndef BOARD_EDGE_FWD_H
#define BOARD_EDGE_FWD_H

#include <functional>

namespace board {

class Edge;
typedef std::reference_wrapper<Edge> EdgeRef;
typedef std::reference_wrapper<const Edge> EdgeCRef;

} // namespace board

#endif // !BOARD_EDGE_FWD_H
