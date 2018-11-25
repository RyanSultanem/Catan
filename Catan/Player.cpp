#include "Player.hpp"

#include <algorithm>

namespace player {

Player::Player(int id) : m_id(id), m_settlmentCount(5), m_points(0), m_settlment(id)
{
}

Player::Player(const Player & player) : m_id(player.m_id), m_settlmentCount(player.m_settlmentCount), m_points(player.m_points), m_settlment(player.m_settlment)
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

std::string Player::serialize() const
{
   return std::to_string(m_id) + ","
      + std::to_string(m_points);
}

} // namespace player