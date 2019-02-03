#include "PlayerReactions.hpp"

#include "Player.hpp"
#include "Building.hpp"

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
	if (!settlementRessourcesAvailable(player)) // TODO: check if really needed
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

} // namespace reactions
} // namespace player

