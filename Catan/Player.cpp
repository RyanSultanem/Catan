#include "Player.hpp"

#include <algorithm>

Player::Player(int id) : m_id(id)
{
}

Player::~Player()
{
}

std::optional<token::building::SettlementRef> Player::getSettlement()
{
   auto settlement = std::find_if(m_settlments.begin(), m_settlments.end(), [](const token::building::Settlement & settlement)
   {
      return !settlement.isOnBoard();
   });

   return settlement != m_settlments.end() ? std::optional<token::building::SettlementRef>(*settlement) : std::nullopt;
}

int Player::getId() const
{
   return m_id;
}
