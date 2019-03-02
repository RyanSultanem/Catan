#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "PlayerInterface.hpp"

#include "Building.hpp"
#include "Road.hpp"

#include <vector>
#include <optional>
#include "Development.hpp"

namespace player {
   
class Player : public Owner, public serialize::Serializable
{
public:
   explicit Player(int id);
   Player(const Player & player);
   ~Player();

   std::optional<token::building::SettlementRef> getSettlement();
   std::optional<token::RoadRef> getRoad();
   std::optional<token::building::CityRef> getCity();

   int getId() const override;
   void receivePoints(int points);

   void increaseSettlmentCount();
   void decreaseSettlmentCount();
   void decreaseRoadCount();
   void decreaseCityCount();

   void addRessource(card::RessourceType ressourceType, unsigned int count);
   bool removeRessource(card::RessourceType ressourceType, unsigned int count);
   int getRessourceCount(card::RessourceType ressourceType) const;
   int getNumberOfRessources() const;
   card::RessourceType removeRandomRessource();

   void receiveDevelopment(std::unique_ptr<card::Development> && development);
   
   void setExchangeCost(card::RessourceType ressourceType, int cost);
   int getExchangeCost(card::RessourceType ressourceType) const;

   std::string serialize() const override;

private:
   int m_id;
   int m_points;

   token::building::Settlement m_settlment;
   int m_settlmentCount;

   token::building::City m_city;
   int m_cityCount;

   token::Road m_road;
   int m_roadCount;

   std::unordered_map<card::RessourceType, int> m_ressources;
   std::unordered_map<card::RessourceType, int> m_exchangeCosts;
};

} // namespace player

#endif // !PLAYER_HPP
