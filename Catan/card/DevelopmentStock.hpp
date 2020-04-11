#ifndef CARD_DEVELOPMENT_STOCK_HPP
#define CARD_DEVELOPMENT_STOCK_HPP

#include <card/Development.hpp>

#include <board/Board.fwd.hpp>

#include <player/Player.fwd.hpp>

#include <utility/NumberGenerator.fwd.hpp>

#include <memory>
#include <optional>
#include <unordered_map>

class DevelopmentStock
{
public:
	DevelopmentStock(const NumberGenerator & numberGenerator);
	std::optional<card::Development> drawCard();
	bool empty() const;
	void initialize(std::vector<player::Player> & players, board::Board & board); // TODO: dependency on board shouldnt be on stock level, maybe players dependecy too.

private:
	std::unordered_map<card::DevelopmentType, int> m_developmentCardCount; // TODO: two maps could be combined as 1 with pair on second element
	std::unordered_map<card::DevelopmentType, std::unique_ptr<card::DevelopmentAction>> m_developmentActions; // TODO: check if can get rid of unique_ptr (used for late initialization mainly..)

	const NumberGenerator & m_numberGenerator;
};

#endif // !CARD_DEVELOPMENT_STOCK_HPP