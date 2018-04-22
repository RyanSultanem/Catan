#include <gtest/gtest.h>

#include "Game.hpp"
#include "Interface.hpp"

class InterfaceStub : public Interface
{
public:
   void printBoard() override
   {
      
   }
   void updateBoard() override
   {
      
   }

   int getBuildingPlacementPosition(int playerId) override
   {
      return 0;
   }

};

TEST(GameTest, canaryTest)
{
   InterfaceStub interface;
   Game game(interface);
   game.play();
}