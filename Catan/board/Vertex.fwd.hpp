#ifndef BOARD_VERTEX_FWD_HPP
#define BOARD_VERTEX_FWD_HPP

#include <functional>

namespace board {

class Vertex;
typedef std::reference_wrapper<Vertex> VertexRef;
typedef std::reference_wrapper<const Vertex> VertexCRef;

} // namespace board

#endif // !BOARD_VERTEX_FWD_HPP
