#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Building.hpp"
#include <vector>
#include <optional>
#include "PlayerInterface.hpp"

namespace player {
   
class Player : public Owner, public serialize::Serializable
{
public:
   explicit Player(int id);
   Player(const Player & player);
   ~Player();

   std::optional<token::building::SettlementRef> getSettlement();
   int getId() const override;
   void receivePoints(int points);
   void increaseSettlmentCount();
   void decreaseSettlmentCount();

   void addRessource(card::RessourceType ressourceType, unsigned int count);
   int getRessourceCount(card::RessourceType ressourceType) const;

   std::string serialize() const override;

private:
   int m_id;
   int m_points;

   token::building::Settlement m_settlment;
   int m_settlmentCount;

   std::unordered_map<card::Ressource, int> m_ressources;
};

} // namespace player

#endif // !PLAYER_HPP
