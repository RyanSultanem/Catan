#ifndef PLAYER_PLAYERREACTIONS_HPP
#define PLAYER_PLAYERREACTIONS_HPP

#include <board/Harbor.fwd.hpp>

#include <card/Card.fwd.hpp>

#include <player/Player.fwd.hpp>
#include <token/Token.fwd.hpp>

#include <functional>
#include <optional>

namespace player {
namespace reactions {

bool ressourcesAvailable(const Player & player, const std::unordered_map<card::RessourceType, int> & costs);

bool tokenRessourcesAvailable(const Player & player, const token::Token & token);
bool tokenPayRessources(Player & player, const token::Token & token);
void tokenPlaced(Player & player, const token::Token & token);

bool performExchangeCards(Player & player, card::RessourceType typeResult, card::RessourceType typeToTrade);
void settlementPlacedOnHarbor(Player & player, const Harbor & harbor);

void stealPlayerCard(Player & receiver, Player & giver, int index);
void stealAllRessources(Player & receiver, Player & sender, card::RessourceType ressource);

bool burnCards(Player & player, const std::unordered_map<card::RessourceType, int> & ressourcesToBurn);

bool developmentRessourceAvailable(const Player & player);
bool developmentPay(Player & player);

} // namespace reactions
} // namespace player

#endif // !PLAYER_PLAYERREACTIONS_HPP