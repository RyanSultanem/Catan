#ifndef BOARD_CELL_FWD_HPP
#define BOARD_CELL_FWD_HPP

#include <functional>

namespace cell {

class Cell;
typedef std::reference_wrapper<Cell> CellRef;
typedef std::reference_wrapper<const Cell> CellCRef;

}

#endif // !BOARD_CELL_FWD_HPP
