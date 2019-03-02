#include "Development.hpp"

namespace {

const std::unordered_map<card::RessourceType, int> DEVELOPMENT_COST = 
{
	{card::RessourceType::ORE, 1},
	{card::RessourceType::GRAIN, 1},
	{card::RessourceType::WOOL, 1}
};

} // namespace anonymous

namespace card {

const std::unordered_map<card::RessourceType, int> & getDevelopmentCost()
{
	return DEVELOPMENT_COST;
}

} // namespace card

