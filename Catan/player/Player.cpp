#include <player/Player.hpp>

#include <card/Development.hpp>
#include <utility/Utility.hpp>

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
	, m_exchangeCosts(5)
	, m_developmentCards()
{
	m_ressources.emplace(card::RessourceType::LUMBER,	0);
	m_ressources.emplace(card::RessourceType::BRICK,	0);
	m_ressources.emplace(card::RessourceType::GRAIN,	0);
	m_ressources.emplace(card::RessourceType::WOOL,		0);
	m_ressources.emplace(card::RessourceType::ORE,		0);

	m_exchangeCosts.emplace(card::RessourceType::LUMBER,	4);
	m_exchangeCosts.emplace(card::RessourceType::BRICK,		4);
	m_exchangeCosts.emplace(card::RessourceType::GRAIN,		4);
	m_exchangeCosts.emplace(card::RessourceType::WOOL,		4);
	m_exchangeCosts.emplace(card::RessourceType::ORE,		4);

	m_developmentCards.reserve(5); // Not necessarily 5. Just to avoid reconstruction everytime.
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
	, m_exchangeCosts(player.m_exchangeCosts)
	, m_developmentCards(player.m_developmentCards)
{
}

Player::~Player()
{
}

std::optional<token::building::SettlementRef> Player::getSettlement()
{
   if (m_settlmentCount > 0)
   {
      return m_settlment;
   }

   return std::nullopt;
}

std::optional<token::RoadRef> Player::getRoad()
{
	if(m_roadCount > 0)
	{
		return m_road;
	}

	return std::nullopt;
}

std::optional<token::building::CityRef> Player::getCity()
{
	if (m_cityCount > 0)
	{
		return m_city;
	}

	return std::nullopt;
}

int Player::getId() const
{
   return m_id;
}

int Player::getPoints() const
{
	return m_points;
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
	m_ressources[ressourceType] += count;
}

bool Player::removeRessource(card::RessourceType ressourceType, unsigned int count)
{
	if (m_ressources[ressourceType] >= count)
	{
		m_ressources[ressourceType] = m_ressources[ressourceType] - count;
		return true;
	}

	return false;
}

int Player::getRessourceCount(card::RessourceType ressourceType) const
{
	return m_ressources.at(ressourceType);
}

int Player::getNumberOfRessources() const
{
	return utility::getCount(m_ressources);
}

std::optional<card::RessourceType> Player::removeRandomRessource(int index)
{
	const auto ressourceIterator = utility::getIndexIterator(m_ressources, index);

	if (ressourceIterator == m_ressources.end())
		return std::nullopt;

	removeRessource(ressourceIterator->first, 1);
	return ressourceIterator->first;
}

void Player::receiveDevelopment(const card::Development & development)
{
	m_developmentCards.push_back(development);
}

std::optional<card::DevelopmentRef> Player::getUnusedDevelopment(card::DevelopmentType developmentType)
{
	std::optional<card::DevelopmentRef> optDevCard(std::nullopt);

	std::for_each(m_developmentCards.begin(), m_developmentCards.end(), 
		[&optDevCard, developmentType](card::Development & development)
	{
		if (!development.isUsed() && development.getType() == developmentType)
			optDevCard = development;
	});

	return optDevCard;
}

std::vector<card::DevelopmentCRef> Player::getUsedDevelopments() const
{
	std::vector<card::DevelopmentCRef> usedDevelopments;
	usedDevelopments.reserve(m_developmentCards.size());

	std::for_each(m_developmentCards.begin(), m_developmentCards.end(), 
		[&usedDevelopments](const card::Development & development)
	{
		if(development.isUsed())
			usedDevelopments.push_back(development);
	});

	return usedDevelopments;
}

void Player::setAllExchangeCosts(int newCost)
{
	std::for_each(m_exchangeCosts.begin(), m_exchangeCosts.end(),
		[this, newCost](const std::pair<card::RessourceType, int>& exchangeCost)
	{
		setExchangeCost(exchangeCost.first, newCost);
	});
}

void Player::setExchangeCost(card::RessourceType ressourceType, int newCost)
{
	if(m_exchangeCosts.at(ressourceType) > newCost)
		m_exchangeCosts[ressourceType] = newCost;
}

int Player::getExchangeCost(card::RessourceType ressourceType) const
{
	return m_exchangeCosts.at(ressourceType);
}

std::string Player::serialize() const
{
	// TODO: std::accumulate
	std::string ressourcesCounts =
		"L: " + std::to_string(m_ressources.at(card::RessourceType::LUMBER)) + " | " +
		"B: " + std::to_string(m_ressources.at(card::RessourceType::BRICK)) + " | " +
		"W: " + std::to_string(m_ressources.at(card::RessourceType::WOOL)) + " | " +
		"G: " + std::to_string(m_ressources.at(card::RessourceType::GRAIN)) + " | " +
		"O: " + std::to_string(m_ressources.at(card::RessourceType::ORE));
	
	std::string devCards = serialize::containerSerialize(m_developmentCards, std::string(), std::string());

	return 
		"ID: " + std::to_string(m_id) + ", " +
		"Points: " + std::to_string(m_points) + ", "+
		"Res: (" + ressourcesCounts + "), " + 
		"Dev: (" + devCards + ")";
}

void Player::decreaseCityCount()
{
	if (m_cityCount - 1 >= 0)
		--m_cityCount;
}

} // namespace player