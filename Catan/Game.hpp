#ifndef GAME_HPP
#define GAME_HPP

#include "Board.h"
#include "Player.hpp"
#include "Interface.hpp"

#include <vector>

class Game
{
public:
   Game(Interface & interface, int numberOfPlayers);
   ~Game();

   void play();

private:
   board::Board m_board;

   std::vector<player::Player> m_players;
   
   Interface & m_interface;

   void setupPlayers(int numberOfPlayers);
   void initialSettlmentPlacement();

   void showStatus();
};

#endif // GAME_HPP