#include <token/Robber.hpp>

#include <board/Cell.hpp>

namespace token {

Robber::Robber()
	: m_cell(std::nullopt)
	, m_cellValue(0)
{
}

void Robber::initializeRobberCell(board::Cell & cell)
{
	m_cell = cell;
	m_cellValue = cell.getNumber();
}

bool Robber::applyTo(board::Cell & newCell)
{	
	if (!m_cell || m_cell->get().getId() == newCell.getId())
		return false;

	resetOldCellValue();
	applyOnNewCell(newCell);

	return true;
}

const std::optional<board::CellRef>& Robber::getCell() const
{
	return m_cell;
}

void Robber::resetOldCellValue()
{
	m_cell->get().setNumber(m_cellValue);
}

void Robber::applyOnNewCell(board::Cell & newCell)
{
	m_cellValue = newCell.getNumber();
	m_cell = newCell;
	m_cell->get().setNumber(0);
}

} // namespace token