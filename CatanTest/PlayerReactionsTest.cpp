#include <gtest/gtest.h>

#include <player/PlayerReactions.hpp>
#include <player/Player.hpp>

#include <board/Harbor.hpp>

TEST(PlayerReactionAvailableRessource, noRessourcesAvialbleRessources)
{
	player::Player player(0);

	std::unordered_map<card::Ressource, int> expectedAvaialble =
	{
		{ card::Ressource::BRICK, 0 },
		{ card::Ressource::GRAIN, 0 },
		{ card::Ressource::LUMBER, 0 }
	};

	EXPECT_EQ(player::reactions::ressourcesAvailable(player, expectedAvaialble), true);
}

TEST(PlayerReactionAvailableRessource, exactAvialbleRessources)
{
	player::Player player(0);

	player.addRessource(card::Ressource::BRICK, 1);
	player.addRessource(card::Ressource::GRAIN, 2);
	player.addRessource(card::Ressource::LUMBER, 3);

	std::unordered_map<card::Ressource, int> expectedAvaialble =
	{
		{ card::Ressource::BRICK, 1 },
		{ card::Ressource::GRAIN, 2 },
		{ card::Ressource::LUMBER, 3 }
	};

	EXPECT_EQ(player::reactions::ressourcesAvailable(player, expectedAvaialble), true);
}

TEST(PlayerReactionAvailableRessource, exceededAvialbleRessources)
{
	player::Player player(0);

	player.addRessource(card::Ressource::BRICK, 1);
	player.addRessource(card::Ressource::GRAIN, 2);
	player.addRessource(card::Ressource::LUMBER, 3);

	std::unordered_map<card::Ressource, int> expectedAvaialble =
	{
		{ card::Ressource::BRICK, 1 },
		{ card::Ressource::GRAIN, 1 }
	};

	EXPECT_EQ(player::reactions::ressourcesAvailable(player, expectedAvaialble), true);
}

TEST(PlayerReactionAvailableRessource, NotAvialbleRessourcesBecauseCountExceed)
{
	player::Player player(0);

	player.addRessource(card::Ressource::BRICK, 1);
	player.addRessource(card::Ressource::GRAIN, 2);
	player.addRessource(card::Ressource::LUMBER, 3);

	std::unordered_map<card::Ressource, int> expectedAvaialble =
	{
		{ card::Ressource::BRICK, 3 },
		{ card::Ressource::GRAIN, 2 }
	};

	EXPECT_EQ(player::reactions::ressourcesAvailable(player, expectedAvaialble), false);
}

TEST(PlayerReactionAvailableRessource, NotAvialbleRessourcesBecauseNotAvailableType)
{
	player::Player player(0);

	player.addRessource(card::Ressource::BRICK, 1);
	player.addRessource(card::Ressource::GRAIN, 2);
	player.addRessource(card::Ressource::LUMBER, 3);

	std::unordered_map<card::Ressource, int> expectedAvaialble =
	{
		{ card::Ressource::BRICK, 1 },
		{ card::Ressource::ORE, 1 }
	};

	EXPECT_EQ(player::reactions::ressourcesAvailable(player, expectedAvaialble), false);
}

TEST(PlayerReactionAvailableRessource, NotAvialbleRessourcesBecauseNoRessourcesAdded)
{
	player::Player player(0);

	std::unordered_map<card::Ressource, int> expectedAvaialble =
	{
		{ card::Ressource::BRICK, 1 },
		{ card::Ressource::ORE, 1 }
	};

	EXPECT_EQ(player::reactions::ressourcesAvailable(player, expectedAvaialble), false);
}

TEST(PlayerReactionBuildingCostAvailable, ExactSettlementAvailable)
{
	player::Player player(0);

	player.addRessource(card::Ressource::BRICK, 1);
	player.addRessource(card::Ressource::GRAIN, 1);
	player.addRessource(card::Ressource::LUMBER, 1);
	player.addRessource(card::Ressource::WOOL, 1);

	EXPECT_EQ(player::reactions::tokenRessourcesAvailable(player, player.getSettlement().value()), true);
}

TEST(PlayerReactionBuildingCostAvailable, SettlementNotAvailableOneRessourceMissing)
{
	player::Player player(0);

	player.addRessource(card::Ressource::GRAIN, 1);
	player.addRessource(card::Ressource::LUMBER, 1);
	player.addRessource(card::Ressource::WOOL, 1);

	EXPECT_EQ(player::reactions::tokenRessourcesAvailable(player, player.getSettlement().value()), false);
}

TEST(PlayerReactionBuildingCostAvailable, ExcessiveSettlementAvailable)
{
	player::Player player(0);

	player.addRessource(card::Ressource::BRICK, 10);
	player.addRessource(card::Ressource::GRAIN, 10);
	player.addRessource(card::Ressource::LUMBER, 10);
	player.addRessource(card::Ressource::WOOL, 10);
	player.addRessource(card::Ressource::ORE, 10);

	EXPECT_EQ(player::reactions::tokenRessourcesAvailable(player, player.getSettlement().value()), true);
}

TEST(PlayerReactionBuildingPay, SuccesfulSettlementPay)
{
	player::Player player(0);

	player.addRessource(card::Ressource::BRICK, 10);
	player.addRessource(card::Ressource::GRAIN, 10);
	player.addRessource(card::Ressource::LUMBER, 10);
	player.addRessource(card::Ressource::WOOL, 10);
	player.addRessource(card::Ressource::ORE, 10);

	bool payResult = player::reactions::tokenPayRessources(player, player.getSettlement().value());

	EXPECT_EQ(payResult, true);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::BRICK), 9);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::GRAIN), 9);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::LUMBER), 9);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::WOOL), 9);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::ORE), 10);
}

TEST(PlayerReactionBuildingPay, SuccesfulSettlementPayExactRessources)
{
	player::Player player(0);

	player.addRessource(card::Ressource::BRICK, 1);
	player.addRessource(card::Ressource::GRAIN, 1);
	player.addRessource(card::Ressource::LUMBER, 1);
	player.addRessource(card::Ressource::WOOL, 1);

	bool payResult = player::reactions::tokenPayRessources(player, player.getSettlement().value());

	EXPECT_EQ(payResult, true);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::BRICK), 0);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::GRAIN), 0);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::LUMBER), 0);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::WOOL), 0);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::ORE), 0);
}

TEST(PlayerReactionBuildingPay, FailSettlementPayNotEnoughRessources)
{
	player::Player player(0);

	player.addRessource(card::Ressource::BRICK, 10);
	player.addRessource(card::Ressource::GRAIN, 10);
	player.addRessource(card::Ressource::WOOL, 10);
	player.addRessource(card::Ressource::ORE, 10);

	bool payResult = player::reactions::tokenPayRessources(player, player.getSettlement().value());

	EXPECT_EQ(payResult, false);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::BRICK), 10);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::GRAIN), 10);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::LUMBER), 0);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::WOOL), 10);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::ORE), 10);
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

TEST(PlayerReactionExchangeCard, Basic4to1)
{
	player::Player player(0);

	player.addRessource(card::Ressource::BRICK, 4);

	bool result = player::reactions::performExchangeCards(player, card::Ressource::ORE, card::Ressource::BRICK);

	EXPECT_TRUE(result);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::BRICK), 0);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::ORE), 1);
}

TEST(PlayerReactionExchangeCard, Basic3to1Fail)
{
	player::Player player(0);

	player.addRessource(card::Ressource::BRICK, 3);

	bool result = player::reactions::performExchangeCards(player, card::Ressource::ORE, card::Ressource::BRICK);

	EXPECT_FALSE(result);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::BRICK), 3);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::ORE), 0);
}

TEST(PlayerReactionExchangeCard, Basic4to1WithExtraRessources)
{
	player::Player player(0);

	player.addRessource(card::Ressource::BRICK, 7);
	player.addRessource(card::Ressource::ORE, 2);

	bool result = player::reactions::performExchangeCards(player, card::Ressource::ORE, card::Ressource::BRICK);

	EXPECT_TRUE(result);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::BRICK), 3);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::ORE), 3);
}

TEST(PlayerReactionSettlementOnHarbor, SpecificRessourceHarbor)
{
	player::Player player(0);

	int oldBrickRate = player.getExchangeCost(card::Ressource::BRICK);
	player::reactions::settlementPlacedOnHarbor(player, Harbor(card::Ressource::BRICK));
	int newBrickRate = player.getExchangeCost(card::Ressource::BRICK);

	EXPECT_NE(oldBrickRate, newBrickRate);
	EXPECT_EQ(newBrickRate, 2);
}

TEST(PlayerReactionSettlementOnHarbor, AllRessourceHarbor)
{
	player::Player player(0);

	player::reactions::settlementPlacedOnHarbor(player, Harbor(card::Ressource::NO_RESSOURCE));

	EXPECT_EQ(player.getExchangeCost(card::Ressource::BRICK), 3);
	EXPECT_EQ(player.getExchangeCost(card::Ressource::WOOL), 3);
	EXPECT_EQ(player.getExchangeCost(card::Ressource::LUMBER), 3);
	EXPECT_EQ(player.getExchangeCost(card::Ressource::GRAIN), 3);
	EXPECT_EQ(player.getExchangeCost(card::Ressource::ORE), 3);
}


TEST(PlayerReactionSettlementOnHarbor, AllRessourceThenSpecificHarbor)
{
	player::Player player(0);

	player::reactions::settlementPlacedOnHarbor(player, Harbor(card::Ressource::NO_RESSOURCE));
	player::reactions::settlementPlacedOnHarbor(player, Harbor(card::Ressource::BRICK));

	EXPECT_EQ(player.getExchangeCost(card::Ressource::BRICK), 2);
	EXPECT_EQ(player.getExchangeCost(card::Ressource::WOOL), 3);
	EXPECT_EQ(player.getExchangeCost(card::Ressource::LUMBER), 3);
	EXPECT_EQ(player.getExchangeCost(card::Ressource::GRAIN), 3);
	EXPECT_EQ(player.getExchangeCost(card::Ressource::ORE), 3);
}

TEST(PlayerReactionSettlementOnHarbor, SpecificThenAllRessourceHarbor)
{
	player::Player player(0);

	player::reactions::settlementPlacedOnHarbor(player, Harbor(card::Ressource::BRICK));
	player::reactions::settlementPlacedOnHarbor(player, Harbor(card::Ressource::NO_RESSOURCE));

	EXPECT_EQ(player.getExchangeCost(card::Ressource::BRICK), 2);
	EXPECT_EQ(player.getExchangeCost(card::Ressource::WOOL), 3);
	EXPECT_EQ(player.getExchangeCost(card::Ressource::LUMBER), 3);
	EXPECT_EQ(player.getExchangeCost(card::Ressource::GRAIN), 3);
	EXPECT_EQ(player.getExchangeCost(card::Ressource::ORE), 3);
}

TEST(PlayerReactionStealPlayerCard, SimpleSteal)
{
	player::Player receiver(0);
	player::Player giver(1);

	giver.addRessource(card::Ressource::BRICK, 1);

	player::reactions::stealPlayerCard(receiver, giver, 0);

	EXPECT_EQ(receiver.getRessourceCount(card::Ressource::BRICK), 1);
	EXPECT_EQ(giver.getRessourceCount(card::Ressource::BRICK), 0);
}

TEST(PlayerReactionStealPlayerCard, StealWithNoCards)
{
	player::Player receiver(0);
	player::Player giver(1);

	receiver.addRessource(card::Ressource::BRICK, 2);

	player::reactions::stealPlayerCard(receiver, giver, 0);

	EXPECT_EQ(receiver.getNumberOfRessources(), 2);
	EXPECT_EQ(giver.getNumberOfRessources(), 0);
}

TEST(PlayerReactionStealPlayerCard, StealFromHimself)
{
	player::Player player(0);

	player.addRessource(card::Ressource::BRICK, 1);

	player::reactions::stealPlayerCard(player, player, 0);

	EXPECT_EQ(player.getNumberOfRessources(), 1);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::BRICK), 1);
}

TEST(PlayerReactionStealAllRessources, SimpleSteal)
{
	player::Player receiver(0);
	player::Player giver(1);

	receiver.addRessource(card::Ressource::BRICK, 2);
	giver.addRessource(card::Ressource::BRICK, 4);
	player::reactions::stealAllRessources(receiver, giver, card::Ressource::BRICK);

	EXPECT_EQ(receiver.getRessourceCount(card::Ressource::BRICK), 6);
	EXPECT_EQ(giver.getRessourceCount(card::Ressource::BRICK), 0);
}

TEST(PlayerReactionBurnCards, SimpleBurn)
{
	player::Player player(0);

	player.addRessource(card::Ressource::BRICK, 4);
	player.addRessource(card::Ressource::WOOL, 3);
	player.addRessource(card::Ressource::ORE, 2);
	player.addRessource(card::Ressource::LUMBER, 2);

	std::unordered_map<card::Ressource, int> ressourceToBurn =
	{
		{ card::Ressource::BRICK, 2 },
		{ card::Ressource::WOOL, 1 },
		{ card::Ressource::ORE, 1 }
	};

	bool success = player::reactions::burnCards(player, ressourceToBurn);

	EXPECT_TRUE(success);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::BRICK), 2);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::WOOL), 2);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::ORE), 1);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::LUMBER), 2);
}

TEST(PlayerReactionBurnCards, NotEnoughRessourcesToBurn)
{
	player::Player player(0);

	player.addRessource(card::Ressource::BRICK, 4);
	player.addRessource(card::Ressource::WOOL, 4);
	player.addRessource(card::Ressource::ORE, 4);
	player.addRessource(card::Ressource::LUMBER, 4);

	std::unordered_map<card::Ressource, int> ressourceToBurn =
	{
		{ card::Ressource::BRICK, 5 },
		{ card::Ressource::WOOL, 5 },
		{ card::Ressource::ORE, 5 }
	};

	bool success = player::reactions::burnCards(player, ressourceToBurn);

	EXPECT_FALSE(success);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::BRICK), 4);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::WOOL), 4);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::ORE), 4);
	EXPECT_EQ(player.getRessourceCount(card::Ressource::LUMBER), 4);
}