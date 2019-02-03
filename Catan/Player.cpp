#include "Player.hpp"

#include <algorithm>

namespace player {

Player::Player(int id)
	: m_id(id)
	, m_points(0)
	, m_settlment(id)
	, m_settlmentCount(5)
	, m_city(id)
	, m_cityCount(4)
	, m_road(id)
	, m_roadCount(15)
	, m_ressources(5)
{
	m_ressources.emplace(card::RessourceType::LUMBER,	0);
	m_ressources.emplace(card::RessourceType::BRICK,	0);
	m_ressources.emplace(card::RessourceType::GRAIN,	0);
	m_ressources.emplace(card::RessourceType::WOOL,		0);
	m_ressources.emplace(card::RessourceType::ORE,		0);
}

Player::Player(const Player & player) 
	: m_id(player.m_id)
	, m_points(player.m_points)
	, m_settlment(player.m_settlment)
	, m_settlmentCount(player.m_settlmentCount)
	, m_city(player.m_city)
	, m_cityCount(player.m_cityCount)
	, m_road(player.m_road)
	, m_roadCount(player.m_roadCount)
	, m_ressources(player.m_ressources)
{
}

Player::~Player()
{
}

std::optional<token::building::SettlementRef> Player::getSettlement()
{
   if (m_settlmentCount > 0)
   {
      return std::optional<token::building::SettlementRef>(m_settlment);
   }

   return std::nullopt;
}

std::optional<token::RoadRef> Player::getRoad()
{
	if(m_roadCount > 0)
	{
		return std::optional<token::RoadRef>(m_road);
	}

	return std::nullopt;
}

std::optional<token::building::CityRef> Player::getCity()
{
	if (m_cityCount > 0)
	{
		return std::optional<token::building::CityRef>(m_city);
	}

	return std::nullopt;
}

int Player::getId() const
{
   return m_id;
}

void player::Player::receivePoints(int points)
{
   m_points += points;
}

void Player::increaseSettlmentCount()
{
   if(m_settlmentCount + 1 < 4)
	   ++m_settlmentCount;
}

void Player::decreaseSettlmentCount()
{
   if(m_settlmentCount - 1 >= 0)
	   --m_settlmentCount;
}

void Player::decreaseRoadCount()
{
	if (m_roadCount - 1 >= 0)
		--m_roadCount;
}

void Player::addRessource(card::RessourceType ressourceType, unsigned int count)
{
	m_ressources[card::Ressource(ressourceType)] += count;
}

void Player::removeRessource(card::RessourceType ressourceType, unsigned int count)
{
	if (m_ressources[card::Ressource(ressourceType)] >= count)
		m_ressources[card::Ressource(ressourceType)] = m_ressources[card::Ressource(ressourceType)] - count;
}

int Player::getRessourceCount(card::RessourceType ressourceType) const
{
	return m_ressources.at(card::Ressource(ressourceType));
}

std::string Player::serialize() const
{
	std::string ressourcesCounts =
		std::to_string(m_ressources.at(card::Ressource(card::RessourceType::LUMBER))) + "|" +
		std::to_string(m_ressources.at(card::Ressource(card::RessourceType::BRICK))) + "|" +
		std::to_string(m_ressources.at(card::Ressource(card::RessourceType::WOOL))) + "|" +
		std::to_string(m_ressources.at(card::Ressource(card::RessourceType::GRAIN))) + "|" +
		std::to_string(m_ressources.at(card::Ressource(card::RessourceType::ORE)));
	
	return std::to_string(m_id) + "," +
		std::to_string(m_points) + ","+
		ressourcesCounts;
}

void Player::decreaseCityCount()
{
	if (m_cityCount - 1 >= 0)
		--m_cityCount;
}

} // namespace player