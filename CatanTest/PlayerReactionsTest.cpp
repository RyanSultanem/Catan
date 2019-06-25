#include <gtest\gtest.h>

#include <player\PlayerReactions.hpp>

#include <player\Player.hpp>

TEST(PlayerReactionAvailableRessource, noRessourcesAvialbleRessources)
{
	player::Player player(0);

	std::unordered_map<card::RessourceType, int> expectedAvaialble =
	{
		{ card::RessourceType::BRICK, 0 },
		{ card::RessourceType::GRAIN, 0 },
		{ card::RessourceType::LUMBER, 0 }
	};

	EXPECT_EQ(player::reactions::ressourcesAvailable(player, expectedAvaialble), true);
}

TEST(PlayerReactionAvailableRessource, exactAvialbleRessources)
{
	player::Player player(0);

	player.addRessource(card::RessourceType::BRICK, 1);
	player.addRessource(card::RessourceType::GRAIN, 2);
	player.addRessource(card::RessourceType::LUMBER, 3);

	std::unordered_map<card::RessourceType, int> expectedAvaialble =
	{
		{ card::RessourceType::BRICK, 1 },
		{ card::RessourceType::GRAIN, 2 },
		{ card::RessourceType::LUMBER, 3 }
	};

	EXPECT_EQ(player::reactions::ressourcesAvailable(player, expectedAvaialble), true);
}

TEST(PlayerReactionAvailableRessource, exceededAvialbleRessources)
{
	player::Player player(0);

	player.addRessource(card::RessourceType::BRICK, 1);
	player.addRessource(card::RessourceType::GRAIN, 2);
	player.addRessource(card::RessourceType::LUMBER, 3);

	std::unordered_map<card::RessourceType, int> expectedAvaialble =
	{
		{ card::RessourceType::BRICK, 1 },
		{ card::RessourceType::GRAIN, 1 }
	};

	EXPECT_EQ(player::reactions::ressourcesAvailable(player, expectedAvaialble), true);
}

TEST(PlayerReactionAvailableRessource, NotAvialbleRessourcesBecauseCountExceed)
{
	player::Player player(0);

	player.addRessource(card::RessourceType::BRICK, 1);
	player.addRessource(card::RessourceType::GRAIN, 2);
	player.addRessource(card::RessourceType::LUMBER, 3);

	std::unordered_map<card::RessourceType, int> expectedAvaialble =
	{
		{ card::RessourceType::BRICK, 3 },
		{ card::RessourceType::GRAIN, 2 }
	};

	EXPECT_EQ(player::reactions::ressourcesAvailable(player, expectedAvaialble), false);
}

TEST(PlayerReactionAvailableRessource, NotAvialbleRessourcesBecauseNotAvailableType)
{
	player::Player player(0);

	player.addRessource(card::RessourceType::BRICK, 1);
	player.addRessource(card::RessourceType::GRAIN, 2);
	player.addRessource(card::RessourceType::LUMBER, 3);

	std::unordered_map<card::RessourceType, int> expectedAvaialble =
	{
		{ card::RessourceType::BRICK, 1 },
		{ card::RessourceType::ORE, 1 }
	};

	EXPECT_EQ(player::reactions::ressourcesAvailable(player, expectedAvaialble), false);
}

TEST(PlayerReactionAvailableRessource, NotAvialbleRessourcesBecauseNoRessourcesAdded)
{
	player::Player player(0);

	std::unordered_map<card::RessourceType, int> expectedAvaialble =
	{
		{ card::RessourceType::BRICK, 1 },
		{ card::RessourceType::ORE, 1 }
	};

	EXPECT_EQ(player::reactions::ressourcesAvailable(player, expectedAvaialble), false);
}

TEST(PlayerReactionBuildingCostAvailable, ExactSettlementAvailable)
{
	player::Player player(0);

	player.addRessource(card::RessourceType::BRICK, 1);
	player.addRessource(card::RessourceType::GRAIN, 1);
	player.addRessource(card::RessourceType::LUMBER, 1);
	player.addRessource(card::RessourceType::WOOL, 1);

	EXPECT_EQ(player::reactions::tokenRessourcesAvailable(player, player.getSettlement().value()), true);
}

TEST(PlayerReactionBuildingCostAvailable, SettlementNotAvailableOneRessourceMissing)
{
	player::Player player(0);

	player.addRessource(card::RessourceType::GRAIN, 1);
	player.addRessource(card::RessourceType::LUMBER, 1);
	player.addRessource(card::RessourceType::WOOL, 1);

	EXPECT_EQ(player::reactions::tokenRessourcesAvailable(player, player.getSettlement().value()), false);
}

TEST(PlayerReactionBuildingCostAvailable, ExcessiveSettlementAvailable)
{
	player::Player player(0);

	player.addRessource(card::RessourceType::BRICK, 10);
	player.addRessource(card::RessourceType::GRAIN, 10);
	player.addRessource(card::RessourceType::LUMBER, 10);
	player.addRessource(card::RessourceType::WOOL, 10);
	player.addRessource(card::RessourceType::ORE, 10);

	EXPECT_EQ(player::reactions::tokenRessourcesAvailable(player, player.getSettlement().value()), true);
}

TEST(PlayerReactionBuildingPay, SuccesfulSettlementPay)
{
	player::Player player(0);

	player.addRessource(card::RessourceType::BRICK, 10);
	player.addRessource(card::RessourceType::GRAIN, 10);
	player.addRessource(card::RessourceType::LUMBER, 10);
	player.addRessource(card::RessourceType::WOOL, 10);
	player.addRessource(card::RessourceType::ORE, 10);

	bool payResult = player::reactions::tokenPayRessources(player, player.getSettlement().value());

	EXPECT_EQ(payResult, true);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::BRICK), 9);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::GRAIN), 9);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::LUMBER), 9);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::WOOL), 9);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::ORE), 10);
}

TEST(PlayerReactionBuildingPay, SuccesfulSettlementPayExactRessources)
{
	player::Player player(0);

	player.addRessource(card::RessourceType::BRICK, 1);
	player.addRessource(card::RessourceType::GRAIN, 1);
	player.addRessource(card::RessourceType::LUMBER, 1);
	player.addRessource(card::RessourceType::WOOL, 1);

	bool payResult = player::reactions::tokenPayRessources(player, player.getSettlement().value());

	EXPECT_EQ(payResult, true);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::BRICK), 0);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::GRAIN), 0);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::LUMBER), 0);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::WOOL), 0);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::ORE), 0);
}

TEST(PlayerReactionBuildingPay, FailSettlementPayNotEnoughRessources)
{
	player::Player player(0);

	player.addRessource(card::RessourceType::BRICK, 10);
	player.addRessource(card::RessourceType::GRAIN, 10);
	player.addRessource(card::RessourceType::WOOL, 10);
	player.addRessource(card::RessourceType::ORE, 10);

	bool payResult = player::reactions::tokenPayRessources(player, player.getSettlement().value());

	EXPECT_EQ(payResult, false);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::BRICK), 10);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::GRAIN), 10);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::LUMBER), 0);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::WOOL), 10);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::ORE), 10);
}

TEST(PlayerReactionBuildingPlaced, SettlementPlaced)
{
	player::Player player(0);

	int oldPlayerPoints = player.getPoints();

	player::reactions::tokenPlaced(player, player.getSettlement().value());

	EXPECT_EQ(player.getPoints(), oldPlayerPoints + 1);
}

TEST(PlayerReactionBuildingPlaced, CityPlaced)
{
	player::Player player(0);

	int oldPlayerPoints = player.getPoints();

	player::reactions::tokenPlaced(player, player.getCity().value());

	EXPECT_EQ(player.getPoints(), oldPlayerPoints + 1);
}

TEST(PlayerReactionBuildingPlaced, SettlementThenCityPlaced)
{
	player::Player player(0);

	player::reactions::tokenPlaced(player, player.getSettlement().value());
	EXPECT_EQ(player.getPoints(), 1);
	player::reactions::tokenPlaced(player, player.getCity().value());
	EXPECT_EQ(player.getPoints(), 2);
}