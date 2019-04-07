#ifndef TOKEN_ROBBER_HPP
#define TOKEN_ROBBER_HPP

#include <board/Cell.fwd.hpp>

namespace token {

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

} // namespace token

#endif // !TOKEN_ROBBER_HPP