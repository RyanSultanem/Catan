#include <player/PlayerReactions.hpp>

#include <card/Development.hpp>

#include <board/Harbor.hpp>

#include <player/Player.hpp>

#include <token/Building.hpp>
#include <token/TokenVistorPlaced.hpp>

//TODO: rename file and namespace

namespace player {
namespace reactions {

namespace {

void playerPayRessources(Player & player, const std::unordered_map<card::RessourceType, int> & costs)
{
	std::for_each(costs.begin(), costs.end(),
		[&player](const std::unordered_map<card::RessourceType, unsigned int>::value_type& ressourceInt)
	{
		player.removeRessource(ressourceInt.first, ressourceInt.second);
	});
}

} // namespace anonymous

bool ressourcesAvailable(const Player & player, const std::unordered_map<card::RessourceType, int> & costs)
{
	bool hasRessources = true;

	std::for_each(costs.begin(), costs.end(),
		[&player, &hasRessources](const std::unordered_map<card::RessourceType, unsigned int>::value_type& ressourceInt)
	{
		if (player.getRessourceCount(ressourceInt.first) < ressourceInt.second)
			hasRessources = false;
	});

	return hasRessources;
}

//bool settlementRessourcesAvailable(const Player & player)
//{
//	const auto & settlmentCost = token::building::Settlement::getSettlementCost();
//	
//	return ressourcesAvailable(player, settlmentCost);
//}
//
//bool settlementPay(Player & player)
//{
//	if (!settlementRessourcesAvailable(player)) // TODO: check if really needed; Actions already makes sure order is correct, or force the order to be here only, and dont expose the "needed" function(like exchange cards example)
//		return false;
//	
//	const auto & settlmentCost = token::building::Settlement::getSettlementCost();
//
//	playerPayRessources(player, settlmentCost);
//	return true;
//}
//
//void settlmentPlaced(Player & player)
//{
//   player.decreaseSettlmentCount();
//   player.receivePoints(token::building::Settlement::getSettlementPoints());
//}
//
//bool roadRessourcesAvailable(const Player & player)
//{
//	const auto & roadCost = token::Road::getRoadCost();
//
//	return ressourcesAvailable(player, roadCost);
//}
//
//bool roadPay(Player & player)
//{
//	if (!roadRessourcesAvailable(player))
//		return false;
//
//	const auto & roadCost = token::Road::getRoadCost();
//
//	playerPayRessources(player, roadCost);
//	return true;
//}
//
//void roadPlaced(Player & player)
//{
//	player.decreaseRoadCount();
//	// TODO: CHECK for longest road here.
//}
//
//bool cityRessourcesAvailable(const Player & player)
//{
//	const auto & cityCost = token::building::City::getCityCost();
//
//	return ressourcesAvailable(player, cityCost);
//}
//
//bool cityPay(Player & player)
//{
//	if (!cityRessourcesAvailable(player))
//		return false;
//
//	const auto & cityCost = token::building::City::getCityCost();
//
//	playerPayRessources(player, cityCost);
//	return true;
//}
//
//void cityPlaced(Player & player)
//{
//	player.decreaseCityCount();
//	player.increaseSettlmentCount();
//	player.receivePoints(token::building::City::getCityPoints() - token::building::Settlement::getSettlementPoints());
//}
//
bool tokenRessourcesAvailable(const Player & player, const token::Token & token)
{
	const auto & tokenCost = token.getCost();

	return ressourcesAvailable(player, tokenCost);
}

bool tokenPayRessources(Player & player, const token::Token & token)
{
	if (!tokenRessourcesAvailable(player, token))
		return false;

	const auto & tokenCost = token.getCost();

	playerPayRessources(player, tokenCost);
	return true;
}

void tokenPlaced(Player & player, const token::Token & token)
{
	token::TokenVisitorPlaced visitorPlaced(player);
	token.accept(visitorPlaced);
}

bool performExchangeCards(Player & player, card::RessourceType ressourceResult, card::RessourceType ressourceToTrade)
{
   if (ressourceToTrade == ressourceResult)
      return false;

	int typeRateChange = player.getExchangeCost(ressourceToTrade);

	if (player.getRessourceCount(ressourceToTrade) < typeRateChange)
		return false;

	player.removeRessource(ressourceToTrade, typeRateChange);
	player.addRessource(ressourceResult, 1);
	return true;
}

void settlementPlacedOnHarbor(Player & player, const Harbor & harbor)
{
	if (harbor.getRessourceType() != card::RessourceType::NO_RESSOURCE)
		player.setExchangeCost(harbor.getRessourceType(), harbor.getNewTradeCost());
	else
		player.setAllExchangeCosts(harbor.getNewTradeCost());
}

void stealPlayerCard(Player & receiver, Player & giver, int index)
{
	if (receiver.getId() == giver.getId())
		return; 

	std::optional<card::RessourceType> randomRessource = giver.removeRandomRessource(index);
	if(randomRessource)
		receiver.addRessource(*randomRessource, 1);
}

void stealAllRessources(Player & receiver, Player & sender, card::RessourceType ressource)
{
   if (receiver.getId() == sender.getId())
      return;

   int ressourceCount = sender.getRessourceCount(ressource);
   sender.removeRessource(ressource, ressourceCount);
   receiver.addRessource(ressource, ressourceCount);
}

bool burnCards(Player & player, const std::unordered_map<card::RessourceType, int> & ressourcesToBurn)
{
	bool success = true;

	std::for_each(ressourcesToBurn.begin(), ressourcesToBurn.end(),
		[&player, &success](auto & element)
	{
		success = success && player.removeRessource(element.first, element.second);
	});

	return success;
}

bool developmentRessourceAvailable(const Player & player)
{
	return ressourcesAvailable(player, card::getDevelopmentCost());
}

bool developmentPay(Player & player)
{
	if (!developmentRessourceAvailable(player))
		return false;

	playerPayRessources(player, card::getDevelopmentCost());
	return true;
}

} // namespace reactions
} // namespace player

