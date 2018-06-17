#include "Player.hpp"

#include <algorithm>

namespace player {

Player::Player(int id) : m_id(id), m_settlmentCount(5), m_points(0), m_settlment(*this)
{
}

Player::~Player()
{
}

std::optional<token::building::SettlementRef> Player::getSettlement()
{
   if (m_settlmentCount >= 0)
   {
      m_settlmentCount--;
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

} // namespace player