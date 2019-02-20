#ifndef EDGE_FWD_H
#define EDGE_FWD_H

#include <functional>

namespace board {

class Edge;
typedef std::reference_wrapper<Edge> EdgeRef;
typedef std::reference_wrapper<const Edge> EdgeCRef;

} // namespace board

#endif // !EDGE_FWD_H
