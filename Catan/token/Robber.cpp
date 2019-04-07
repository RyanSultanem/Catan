#include <token/Robber.hpp>

#include <board/Cell.hpp>

namespace token {

Robber::Robber()
	: m_cell(nullptr)
	, m_cellValue(0)
{
}

void Robber::initializeRobberCell(cell::Cell & cell)
{
	m_cell = &cell;
	m_cellValue = cell.getNumber();
}

bool Robber::applyTo(cell::Cell & newCell)
{	
	if (!m_cell || m_cell->getId() == newCell.getId())
		return false;

	resetOldCellValue();
	applyOnNewCell(newCell);

	return true;
}

void Robber::resetOldCellValue()
{
	m_cell->setNumber(m_cellValue);
}

void Robber::applyOnNewCell(cell::Cell & newCell)
{
	m_cellValue = newCell.getNumber();
	m_cell = &newCell;
	m_cell->setNumber(0);
}

} // namespace token