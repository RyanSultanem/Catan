#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Building.hpp"
#include <vector>
#include <optional>

class Player
{
public:
   Player(int id);
   ~Player();

   std::optional<token::building::SettlementRef> getSettlement();
   int getId() const;

private:
   int m_id;
   std::vector<token::building::Settlement> m_settlments;
};

#endif // !PLAYER_HPP
