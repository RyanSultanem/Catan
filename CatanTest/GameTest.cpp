#include <gtest/gtest.h>

#include "Game.hpp"
#include "Interface.hpp"
#include <iostream>

class InterfaceStub : public Interface
{
public:
   void printBoard(const std::string & board) override
   {
      std::cout << board << std::endl;
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