#ifndef PLAYER_REACTIONS_HPP
#define PLAYER_REACTIONS_HPP

#include "Player.fwd.hpp"
#include "Harbor.fwd.hpp"
#include <optional>
#include "Card.fwd.h"
#include <functional>

namespace player {
namespace reactions {

bool ressourcesAvailable(const Player & player, const std::unordered_map<card::RessourceType, int> & costs);

	//TODO: refactor to use Building* + Visitor on Token for XXPlaced Functions
bool settlementRessourcesAvailable(const Player & player);
bool settlementPay(Player & player);
void settlmentPlaced(Player & player);

bool roadRessourcesAvailable(const Player & player);
bool roadPay(Player & player);
void roadPlaced(Player & player);

bool cityRessourcesAvailable(const Player & player);
bool cityPay(Player & player);
void cityPlaced(Player & player);
// -------

bool performExchangeCards(Player & player, int typeResult, int typeToTrade);
void settlementPlacedOnHarbor(Player & player, const Harbor & harbor);

void stealPlayerCard(Player & receiver, Player & giver, int index);
void stealAllRessources(Player & receiver, Player & sender, card::RessourceType ressource);

bool burnCards(Player & player, const std::unordered_map<card::RessourceType, int> & ressourcesToBurn);

bool developmentRessourceAvailable(const Player & player);
bool developmentPay(Player & player);

} // namespace reactions
} // namespace player

#endif // !PLAYER_REACTIONS_HPP