#include "gtest/gtest.h"

#include <player/Player.hpp>

TEST(PlayerTest, NoRessources)
{
	player::Player player(0);

	EXPECT_EQ(player.getRessourceCount(card::Ressource::WOOL), 0);
}

TEST(PlayerTest, AddingRessources)
{
	player::Player player(0);

	player.addRessource(card::Ressource::ORE, 5);
	player.addRessource(card::Ressource::ORE, 3);

	EXPECT_EQ(player.getRessourceCount(card::Ressource::ORE), 8);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::WOOL), 0);
}