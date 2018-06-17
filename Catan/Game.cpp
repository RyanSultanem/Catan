#include "Game.hpp"

#include <algorithm>

Game::Game(Interface & interface, int numberOfPlayers)
   : m_interface(interface), m_numberOfPlayers(numberOfPlayers)
{
   m_players.reserve(numberOfPlayers);
}


Game::~Game()
{
}

void Game::play()
{
   setupPlayers();

   m_interface.printBoard(m_board.serialize());

   initialSettlmentPlacement();

   //m_interface.updateBoard();
   m_interface.printBoard(m_board.serialize());
}

void Game::setupPlayers()
{
   for(int i = 0; i < m_numberOfPlayers; ++i)
      m_players.push_back(player::Player(m_playerId++));
}

void Game::initialSettlmentPlacement()
{
   auto placeSettlement = [&](player::Player & player)
   {
      bool success = false;
      
      while(!success)
      {
         std::optional<token::building::SettlementRef> settlement = player.getSettlement();
         if (settlement)
         {
            int position = m_interface.getBuildingPlacementPosition(player.getId());
            success = m_board.placeSettlement(position, settlement.value());
         }   
      }
   };

   std::for_each(m_players.begin(), m_players.end(), placeSettlement);
   std::for_each(m_players.rbegin(), m_players.rend(), placeSettlement);
}
