#include "Player.hpp"

#include <algorithm>

namespace player {

Player::Player(int id)
	: m_id(id)
	, m_settlmentCount(5)
	, m_points(0)
	, m_settlment(id)
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
	, m_settlmentCount(player.m_settlmentCount)
	, m_points(player.m_points)
	, m_settlment(player.m_settlment)
	, m_ressources(player.m_ressources)
{
}

Player::~Player()
{
}

std::optional<token::building::SettlementRef> Player::getSettlement()
{
   if (m_settlmentCount >= 0)
   {
      return std::optional<token::building::SettlementRef>(m_settlment);
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
      m_settlmentCount++;
}

void Player::decreaseSettlmentCount()
{
   if(m_settlmentCount - 1 < 0)
      m_settlmentCount--;
}

void Player::addRessource(card::RessourceType ressourceType, unsigned int count)
{
	m_ressources[card::Ressource(ressourceType)] += count;
}

int Player::getRessourceCount(card::RessourceType ressourceType) const
{
	return m_ressources.at(card::Ressource(ressourceType));
}

std::string Player::serialize() const
{
	std::string ressourcesCounts =
		std::to_string(m_ressources.at(card::Ressource(card::RessourceType::LUMBER))) + "|"
		+ std::to_string(m_ressources.at(card::Ressource(card::RessourceType::BRICK))) + "|"
		+ std::to_string(m_ressources.at(card::Ressource(card::RessourceType::WOOL))) + "|"
		+ std::to_string(m_ressources.at(card::Ressource(card::RessourceType::GRAIN))) + "|"
		+ std::to_string(m_ressources.at(card::Ressource(card::RessourceType::ORE)));
	
	return std::to_string(m_id) + ","
      + std::to_string(m_points) + ","
	+ ressourcesCounts;
}

} // namespace player