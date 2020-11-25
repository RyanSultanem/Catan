#ifndef TOKEN_ROBBER_HPP
#define TOKEN_ROBBER_HPP

#include <board/Cell.fwd.hpp>

#include <optional>

namespace token {

class Robber
{
public:
	Robber();
	void initializeRobberCell(board::Cell & cell);
	bool applyTo(board::Cell & newCell);

	const std::optional<board::CellRef> & getCell() const;

private:
	std::optional<board::CellRef> m_cell; // TODO: check if can do better
	int m_cellValue;

	void resetOldCellValue();
	void applyOnNewCell(board::Cell & newCell);
};

} // namespace token

#endif // !TOKEN_ROBBER_HPP