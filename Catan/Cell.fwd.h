#ifndef CELL_FWD_H
#define CELL_FWD_H

#include <functional>

namespace cell {

class Cell;
typedef std::reference_wrapper<Cell> CellRef;
typedef std::reference_wrapper<const Cell> CellCRef;

}

#endif // !CELL_FWD_H
