#include "DevelopmentStock.hpp"

#include "Utility.hpp"

DevelopmentStock::DevelopmentStock()
	: m_developmentCardCount(5)
	, m_developmentActions(5)
{
}

std::optional<card::Development> DevelopmentStock::drawCard()
{
	auto iteratorType = utility::getRandomIterator(m_developmentCardCount);

	if (iteratorType != m_developmentCardCount.end() && m_developmentActions.at(iteratorType->first))
		return std::nullopt;

	iteratorType->second--;
	return card::Development(*m_developmentActions.at(iteratorType->first));
}

bool DevelopmentStock::empty() const
{
	return utility::getCount(m_developmentCardCount) != 0;
}

void DevelopmentStock::initialize(std::vector<player::Player> & players, board::Board & board)
{
	m_developmentCardCount.emplace(card::DevelopmentType::Knight,				15);
	m_developmentCardCount.emplace(card::DevelopmentType::FreeRessources,		2);
	m_developmentCardCount.emplace(card::DevelopmentType::BuildTwoFreeRoads,	2);
	m_developmentCardCount.emplace(card::DevelopmentType::Monopoly,				2);
	m_developmentCardCount.emplace(card::DevelopmentType::VictoryPoint,			5);

	m_developmentActions.emplace(card::DevelopmentType::Knight,				std::make_unique<card::KnightAction>(players, board));
	m_developmentActions.emplace(card::DevelopmentType::FreeRessources,		std::make_unique<card::FreeRessourcesAction>());
	m_developmentActions.emplace(card::DevelopmentType::BuildTwoFreeRoads,	std::make_unique<card::BuildTwoFreeRoadsAction>(board));
	m_developmentActions.emplace(card::DevelopmentType::Monopoly,			std::make_unique<card::MonopolyAction>(players));
	m_developmentActions.emplace(card::DevelopmentType::VictoryPoint,		std::make_unique<card::VictoryPointAction>());	
}
