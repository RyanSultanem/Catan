#ifndef VERTEX_FWD_H
#define VERTEX_FWD_H

#include <functional>

namespace board {

class Vertex;
typedef std::reference_wrapper<Vertex> VertexRef;
typedef std::reference_wrapper<const Vertex> VertexCRef;

} // namespace board

#endif // !VERTEX_FWD_H
