#ifndef ROBBER_HPP
#define ROBBER_HPP

#include "Cell.fwd.h"

namespace board {

class Robber
{
public:
	Robber();
	void initializeRobberCell(cell::Cell & cell);
	bool applyTo(cell::Cell & newCell);

private:
	cell::Cell * m_cell; // TODO: check if can do better
	int m_cellValue;

	void resetOldCellValue();
	void applyOnNewCell(cell::Cell & newCell);
};

} // namespace Robber

#endif // !ROBBER_HPP