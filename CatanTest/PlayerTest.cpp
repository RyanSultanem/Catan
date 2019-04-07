#include "gtest/gtest.h"

#include <player/Player.hpp>

TEST(PlayerTest, NoRessources)
{
	player::Player player(0);

	EXPECT_EQ(player.getRessourceCount(card::RessourceType::WOOL), 0);
}

TEST(PlayerTest, AddingRessources)
{
	player::Player player(0);

	player.addRessource(card::RessourceType::ORE, 5);
	player.addRessource(card::RessourceType::ORE, 3);

	EXPECT_EQ(player.getRessourceCount(card::RessourceType::ORE), 8);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::WOOL), 0);
}