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
      int position;

      std::cout << "Choose a position for initial settlment for Player " << playerId << ": ";
      std::cin >> position;

      return position;
   }

};

TEST(GameTest, canaryTest)
{
   InterfaceStub interface;
   Game game(interface, 3);
   game.play();
}