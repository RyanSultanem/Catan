#include <gtest/gtest.h>

#include "Game.hpp"
#include "Interface.hpp"
#include <iostream>

class InterfaceStub : public Interface
{
public:
   void printBoard(const std::string & board) override
   {
      std::cout << "Board Print Start" << std::endl << std::endl;
      std::cout << board << std::endl;
      std::cout << "Board Print End" << std::endl << std::endl;
   }
   void updateBoard() override
   {
      
   }

   int getBuildingPlacementPosition(int playerId) override
   {
      /*int position;

      std::cout << "Choose a position for initial settlment for Player " << playerId << ": ";
      std::cin >> position;

      return position;*/

      int position = -1;

      switch (m_placementCount)
      {
      case 0: position = 1; break;
      case 1: position = 2; break;
      case 2: position = 3; break;
      case 3: position = 4; break;
      case 4: position = 5; break;
      case 5: position = 6; break; 

      default: return -1;
      }

      m_placementCount++;
      return position;
   }

   void printPlayerInfos(const std::string & playersInfo) override
   {
      std::cout << "PlayerInfo Print Start" << std::endl << std::endl;
      std::cout << playersInfo << std::endl;
      std::cout << "PlayerInfo Print End" << std::endl << std::endl;
   }

private:
   int m_placementCount = 0;
};

TEST(GameTest, canaryTest)
{
   InterfaceStub interface;
   Game game(interface, 3);
   game.play();
}