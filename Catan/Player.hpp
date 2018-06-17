#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Building.hpp"
#include <vector>
#include <optional>
#include "PlayerInterface.hpp"

namespace player {
   
class Player : public PointReceiver
{
public:
   explicit Player(int id);
   Player(const Player & player);
   ~Player();

   std::optional<token::building::SettlementRef> getSettlement();
   int getId() const override;
   void receivePoints(int points) override;
   void increaseSettlmentCount() override;
   void decreaseSettlmentCount() override;

private:
   int m_id;
   int m_points;
   token::building::Settlement m_settlment;
   int m_settlmentCount;
};

} // namespace player

#endif // !PLAYER_HPP
