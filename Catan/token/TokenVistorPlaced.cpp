#include <token/TokenVistorPlaced.hpp>

#include <player/Player.hpp>
#include <token/Building.hpp>

token::TokenVisitorPlaced::TokenVisitorPlaced(player::Player & player)
	: m_player(player)
{
}

void token::TokenVisitorPlaced::visit(const building::Settlement & settlement) const
{
	m_player.decreaseSettlmentCount();
	m_player.receivePoints(settlement.getPoints());
}

void token::TokenVisitorPlaced::visit(const building::City & city) const
{
	m_player.decreaseCityCount();
	m_player.increaseSettlmentCount();
	m_player.receivePoints(city.getPoints() - m_player.getSettlement().value().get().getPoints());
}

void token::TokenVisitorPlaced::visit(const Road & road) const
{
	m_player.decreaseRoadCount();
	// TODO: CHECK for longest road here.
}
