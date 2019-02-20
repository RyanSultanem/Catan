#include "PlayerReactions.hpp"

#include "Player.hpp"
#include "Building.hpp"
#include "Harbor.hpp"

namespace player {
namespace reactions {

namespace {
	
bool playerRessourcesAvailable(const Player & player, const std::unordered_map<card::Ressource,int> & costs)
{
	bool hasRessources = true;

	std::for_each(costs.begin(), costs.end(),
		[&player, &hasRessources](const std::unordered_map<card::Ressource, unsigned int>::value_type& ressourceInt)
	{
		if (player.getRessourceCount(ressourceInt.first.getType()) < ressourceInt.second)
			hasRessources = false;
	});

	return hasRessources;
}

void playerPayRessources(Player & player, const std::unordered_map<card::Ressource, int> & costs)
{
	std::for_each(costs.begin(), costs.end(),
		[&player](const std::unordered_map<card::Ressource, unsigned int>::value_type& ressourceInt)
	{
		player.removeRessource(ressourceInt.first.getType(), ressourceInt.second);
	});
}

} // namespace anonymous

bool settlementRessourcesAvailable(const Player & player)
{
	const auto & settlmentCost = token::building::Settlement::getSettlementCost();
	
	return playerRessourcesAvailable(player, settlmentCost);
}

bool settlementPay(Player & player)
{
	if (!settlementRessourcesAvailable(player)) // TODO: check if really needed; Actions already makes sure order is correct, or force the order to be here only, and dont expose the "needed" function(like exchange cards example)
		return false;
	
	const auto & settlmentCost = token::building::Settlement::getSettlementCost();

	playerPayRessources(player, settlmentCost);
	return true;
}

void settlmentPlaced(Player & player)
{
   player.decreaseSettlmentCount();
   player.receivePoints(token::building::Settlement::getSettlementPoints());
}

bool roadRessourcesAvailable(const Player & player)
{
	const auto & roadCost = token::Road::getRoadCost();

	return playerRessourcesAvailable(player, roadCost);
}

bool roadPay(Player & player)
{
	if (!roadRessourcesAvailable(player))
		return false;

	const auto & roadCost = token::Road::getRoadCost();

	playerPayRessources(player, roadCost);
	return true;
}

void roadPlaced(Player & player)
{
	player.decreaseRoadCount();
	// TODO: CHECK for longest road here.
}

bool cityRessourcesAvailable(const Player & player)
{
	const auto & cityCost = token::building::City::getCityCost();

	return playerRessourcesAvailable(player, cityCost);
}

bool cityPay(Player & player)
{
	if (!cityRessourcesAvailable(player))
		return false;

	const auto & cityCost = token::building::City::getCityCost();

	playerPayRessources(player, cityCost);
	return true;
}

void cityPlaced(Player & player)
{
	player.decreaseCityCount();
	player.increaseSettlmentCount();
	player.receivePoints(token::building::City::getCityPoints() - token::building::Settlement::getSettlementPoints());
}

bool performExchangeCards(Player & player, int typeResult, int typeToTrade)
{
	// TODO: remove static_cast by replacing the int with the correct type higher in the stack.
	card::RessourceType ressourceToTrade(static_cast<card::RessourceType>(typeToTrade));
	card::RessourceType ressourceResult(static_cast<card::RessourceType>(typeResult));

	int typeRateChange = player.getExchangeCost(ressourceToTrade);

	if (player.getRessourceCount(ressourceToTrade) < typeRateChange)
		return false;

	player.removeRessource(ressourceToTrade, typeRateChange);
	player.addRessource(ressourceResult, 1);
	return true;
}

void settlementPlacedOnHarbor(Player & player, const Harbor & harbor)
{
	player.setExchangeCost(harbor.getRessourceType(), harbor.getNewTradeCost());
}

} // namespace reactions
} // namespace player

