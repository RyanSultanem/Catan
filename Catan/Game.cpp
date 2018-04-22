#include "Game.hpp"

#include <algorithm>

Game::Game(Interface & interface)
   : m_interface(interface)
{
}


Game::~Game()
{
}

void Game::play()
{
   setupPlayers();

   m_interface.printBoard();

   initialSettlmentPlacement();

   m_interface.updateBoard();


}

void Game::setupPlayers()
{
}

void Game::initialSettlmentPlacement()
{
   auto placeSettlement = [&](Player & player)
   {
      bool success = false;
      
      while(success)
      {
         std::optional<token::building::SettlementRef> settlement = player.getSettlement();
         if(settlement)
         {
            int position = m_interface.getBuildingPlacementPosition(player.getId());
            success = m_board.placeSettlement(position, settlement.value());
         }
         else
         {
            success = false;
         }
         
      }
   };

   std::for_each(m_players.begin(), m_players.end(), placeSettlement);
   std::for_each(m_players.rbegin(), m_players.rend(), placeSettlement);
}
