#include "Game.hpp"

#include "PlayerActions.hpp"

#include <algorithm>

Game::Game(Interface & interface, int numberOfPlayers)
   : m_interface(interface)
{
   setupPlayers(numberOfPlayers);
}

Game::~Game()
{
}

void Game::play()
{
   showStatus();

   initialSettlmentPlacement();

   showStatus();
}

void Game::setupPlayers(int numberOfPlayers)
{
   m_players.reserve(numberOfPlayers);
   for(int i = 0; i < numberOfPlayers; ++i)
      m_players.push_back(player::Player(i));
}

void Game::initialSettlmentPlacement()
{
   auto placeSettlement = [&](player::Player & player) -> int
   {
      std::optional<token::building::SettlementRef> settlementOpt = player.getSettlement();
      if (settlementOpt)
      {
         token::building::Settlement & settlement = settlementOpt.value().get();
         bool success = false;

         while(!success)
         {
            int position = m_interface.getBuildingPlacementPosition(player.getId());
            success = m_board.placeSettlement(position, settlement);

            if(success)
            {
               //player::Player & player = m_players[settlement.getReference()]; //or find
            	player::actions::playerSettlmentPlacementReaction(player, settlement);

				return position;
            }
         }   
      }

	  return -1;
   };

	auto placeSettlementWithRessources = [&](player::Player & player) -> void
	{
		int position = placeSettlement(player);

		if(position != -1)
		{
			const std::vector<card::RessourceType> ressources = m_board.getRessourcesFromVertexPosition(position);
			std::for_each(ressources.begin(), ressources.end(),
				[&player](const card::RessourceType & ressourceType)
			{
				player.addRessource(ressourceType, 1);
			});
		}	
	};

   std::for_each(m_players.begin(), m_players.end(), placeSettlement);
   std::for_each(m_players.rbegin(), m_players.rend(), placeSettlementWithRessources);
}

void Game::showStatus()
{
   m_interface.printBoard(m_board.serialize());
   m_interface.printPlayerInfos(serialize::containerSerialize(m_players, "", "Players: "));
}
