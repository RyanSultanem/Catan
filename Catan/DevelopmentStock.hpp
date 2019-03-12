#ifndef DEVELOPMENT_STOCK_HPP
#define DEVELOPMENT_STOCK_HPP

#include "Development.hpp"

#include "Board.fwd.h"
#include "Player.fwd.hpp"
#include "NumberGenerator.fwd.hpp"

#include <memory>
#include <optional>
#include <unordered_map>

class DevelopmentStock
{
public:
	DevelopmentStock(const NumberGenerator & numberGenerator);
	std::optional<card::Development> drawCard();
	bool empty() const;
	void initialize(std::vector<player::Player> & players, board::Board & board);

private:
	std::unordered_map<card::DevelopmentType, int> m_developmentCardCount; // TODO: two maps could be combined as 1 with pair on second element
	std::unordered_map<card::DevelopmentType, std::unique_ptr<card::DevelopmentAction>> m_developmentActions; // TODO: check if can get rid of unique_ptr (used for late initialization mainly..)

	const NumberGenerator & m_numberGenerator;
};

#endif // !DEVELOPMENT_STOCK_HPP