#ifndef PLAYER_PLAYER_HPP
#define PLAYER_PLAYER_HPP

#include <player/PlayerInterface.hpp>

#include <card/Development.fwd.hpp>

#include <token/Building.hpp>
#include <token/Road.hpp>

#include <utility/Serializable.hpp>

#include <optional>
#include <vector>

namespace player {
   
class Player : public Owner, public serialize::Serializable
{
public:
	explicit Player(int id);
	Player(const Player & player);
	~Player();

	std::optional<token::building::SettlementRef> getSettlement();
	std::optional<token::building::CityRef> getCity();
	std::optional<token::RoadRef> getRoad();

	int getId() const override;
	int getPoints() const;
	void receivePoints(int points);

	void increaseSettlmentCount();
	void decreaseSettlmentCount();
	void decreaseRoadCount();
	void decreaseCityCount();

	void addRessource(card::RessourceType ressourceType, unsigned int count);
	bool removeRessource(card::RessourceType ressourceType, unsigned int count);
	int getRessourceCount(card::RessourceType ressourceType) const;
	int getNumberOfRessources() const;
	std::optional<card::RessourceType> removeRessourceAtIndex(int index);

	void receiveDevelopment(const card::Development & development);
	std::optional<card::DevelopmentRef> getUnusedDevelopment(card::DevelopmentType developmentType);
	std::vector<card::DevelopmentCRef> getUsedDevelopments() const;
   
	void setAllExchangeCosts(int newCost);
	void setExchangeCost(card::RessourceType ressourceType, int newCost);
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
	std::vector<card::Development> m_developmentCards;
};

} // namespace player

#endif // !PLAYER_PLAYER_HPP
