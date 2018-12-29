#include "Game.hpp"

#include "Dice.hpp"
#include "PlayerReactions.hpp"

#include "Building.hpp"

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

	playTurns();
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

			 // TODO: Need to check that no two adjacent Settlement were placed.
            if(success)
            {
               //player::Player & player = m_players[settlement.getReference()]; //or find
            	player::reactions::settlmentPlacement(player, settlement);

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
   m_interface.showBoard(m_board.serialize());
   m_interface.showPlayerInfos(serialize::containerSerialize(m_players, "", "Players: "));
}

void Game::giveRessources(int value)
{
	const std::vector<cell::CellRef> activeCells = m_board.getCellsWithNumber(value);

	std::for_each(activeCells.begin(), activeCells.end(),
		[this](const cell::Cell & activeCell)
	{
		const std::vector<token::building::Building*> activeBuildings = activeCell.getActiveBuildings();

		std::for_each(activeBuildings.begin(), activeBuildings.end(),
			[this, &activeCell](const token::building::Building * activeBuilding)
		{
			// TODO: check if reference is equivalent to position in vector
			player::Player & player = m_players.at(activeBuilding->getReference());
			player.addRessource(activeCell.produceLandRessource().getType(), activeBuilding->getPoints());
		});
	});
}

void Game::checkGameEnded(int turnCount)
{
	// TODO: used only to end game now. should do the real job someday
	if (turnCount >= 15)
		m_gameEnded = true;
}

void Game::playTurns()
{
	board::Dice dice;
	int turnCount = 0;

	while(!m_gameEnded)
	{
		int diceValue = dice.roll();

		giveRessources(diceValue);

		player::Player & activePlayer = m_players.at(turnCount % m_players.size());

		++turnCount;
		checkGameEnded(turnCount);
	}
}
