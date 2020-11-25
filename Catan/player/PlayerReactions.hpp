#ifndef PLAYER_PLAYERREACTIONS_HPP
#define PLAYER_PLAYERREACTIONS_HPP

#include <board/Harbor.fwd.hpp>

#include <card/Card.fwd.hpp>

#include <player/Player.fwd.hpp>
#include <token/Token.fwd.hpp>

#include <functional>
#include <optional>
#include <vector>

namespace player {
namespace reactions {

bool ressourcesAvailable(const Player & player, const std::unordered_map<card::Ressource, int> & costs);
void receiveRessources(Player & player, const std::vector<card::Ressource> & ressources);

bool tokenRessourcesAvailable(const Player & player, const token::Token & token);
bool tokenPayRessources(Player & player, const token::Token & token);
void tokenPlaced(Player & player, const token::Token & token);

bool performExchangeCards(Player & player, card::Ressource typeResult, card::Ressource typeToTrade);
void settlementPlacedOnHarbor(Player & player, const Harbor & harbor);

void stealPlayerCard(Player & receiver, Player & giver, int randomIndex);
void stealAllRessources(Player & receiver, Player & sender, card::Ressource ressource);

bool burnCards(Player & player, const std::unordered_map<card::Ressource, int> & ressourcesToBurn);

bool developmentRessourceAvailable(const Player & player);
bool developmentPay(Player & player);

} // namespace reactions
} // namespace player

#endif // !PLAYER_PLAYERREACTIONS_HPP