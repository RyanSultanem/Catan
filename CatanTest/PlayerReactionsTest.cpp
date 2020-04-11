#include <gtest/gtest.h>

#include <player/PlayerReactions.hpp>
#include <player/Player.hpp>

#include <board/Harbor.hpp>

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

TEST(PlayerReactionExchangeCard, Basic4to1)
{
	player::Player player(0);

	player.addRessource(card::RessourceType::BRICK, 4);

	bool result = player::reactions::performExchangeCards(player, card::RessourceType::ORE, card::RessourceType::BRICK);

	EXPECT_TRUE(result);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::BRICK), 0);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::ORE), 1);
}

TEST(PlayerReactionExchangeCard, Basic3to1Fail)
{
	player::Player player(0);

	player.addRessource(card::RessourceType::BRICK, 3);

	bool result = player::reactions::performExchangeCards(player, card::RessourceType::ORE, card::RessourceType::BRICK);

	EXPECT_FALSE(result);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::BRICK), 3);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::ORE), 0);
}

TEST(PlayerReactionExchangeCard, Basic4to1WithExtraRessources)
{
	player::Player player(0);

	player.addRessource(card::RessourceType::BRICK, 7);
	player.addRessource(card::RessourceType::ORE, 2);

	bool result = player::reactions::performExchangeCards(player, card::RessourceType::ORE, card::RessourceType::BRICK);

	EXPECT_TRUE(result);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::BRICK), 3);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::ORE), 3);
}

TEST(PlayerReactionSettlementOnHarbor, SpecificRessourceHarbor)
{
	player::Player player(0);

	int oldBrickRate = player.getExchangeCost(card::RessourceType::BRICK);
	player::reactions::settlementPlacedOnHarbor(player, Harbor(card::RessourceType::BRICK));
	int newBrickRate = player.getExchangeCost(card::RessourceType::BRICK);

	EXPECT_NE(oldBrickRate, newBrickRate);
	EXPECT_EQ(newBrickRate, 2);
}

TEST(PlayerReactionSettlementOnHarbor, AllRessourceHarbor)
{
	player::Player player(0);

	player::reactions::settlementPlacedOnHarbor(player, Harbor(card::RessourceType::NO_RESSOURCE));

	EXPECT_EQ(player.getExchangeCost(card::RessourceType::BRICK), 3);
	EXPECT_EQ(player.getExchangeCost(card::RessourceType::WOOL), 3);
	EXPECT_EQ(player.getExchangeCost(card::RessourceType::LUMBER), 3);
	EXPECT_EQ(player.getExchangeCost(card::RessourceType::GRAIN), 3);
	EXPECT_EQ(player.getExchangeCost(card::RessourceType::ORE), 3);
}


TEST(PlayerReactionSettlementOnHarbor, AllRessourceThenSpecificHarbor)
{
	player::Player player(0);

	player::reactions::settlementPlacedOnHarbor(player, Harbor(card::RessourceType::NO_RESSOURCE));
	player::reactions::settlementPlacedOnHarbor(player, Harbor(card::RessourceType::BRICK));

	EXPECT_EQ(player.getExchangeCost(card::RessourceType::BRICK), 2);
	EXPECT_EQ(player.getExchangeCost(card::RessourceType::WOOL), 3);
	EXPECT_EQ(player.getExchangeCost(card::RessourceType::LUMBER), 3);
	EXPECT_EQ(player.getExchangeCost(card::RessourceType::GRAIN), 3);
	EXPECT_EQ(player.getExchangeCost(card::RessourceType::ORE), 3);
}

TEST(PlayerReactionSettlementOnHarbor, SpecificThenAllRessourceHarbor)
{
	player::Player player(0);

	player::reactions::settlementPlacedOnHarbor(player, Harbor(card::RessourceType::BRICK));
	player::reactions::settlementPlacedOnHarbor(player, Harbor(card::RessourceType::NO_RESSOURCE));

	EXPECT_EQ(player.getExchangeCost(card::RessourceType::BRICK), 2);
	EXPECT_EQ(player.getExchangeCost(card::RessourceType::WOOL), 3);
	EXPECT_EQ(player.getExchangeCost(card::RessourceType::LUMBER), 3);
	EXPECT_EQ(player.getExchangeCost(card::RessourceType::GRAIN), 3);
	EXPECT_EQ(player.getExchangeCost(card::RessourceType::ORE), 3);
}

TEST(PlayerReactionStealPlayerCard, SimpleSteal)
{
	player::Player receiver(0);
	player::Player giver(1);

	giver.addRessource(card::RessourceType::BRICK, 1);

	player::reactions::stealPlayerCard(receiver, giver, 0);

	EXPECT_EQ(receiver.getRessourceCount(card::RessourceType::BRICK), 1);
	EXPECT_EQ(giver.getRessourceCount(card::RessourceType::BRICK), 0);
}

TEST(PlayerReactionStealPlayerCard, StealWithNoCards)
{
	player::Player receiver(0);
	player::Player giver(1);

	receiver.addRessource(card::RessourceType::BRICK, 2);

	player::reactions::stealPlayerCard(receiver, giver, 0);

	EXPECT_EQ(receiver.getNumberOfRessources(), 2);
	EXPECT_EQ(giver.getNumberOfRessources(), 0);
}

TEST(PlayerReactionStealPlayerCard, StealFromHimself)
{
	player::Player player(0);

	player.addRessource(card::RessourceType::BRICK, 1);

	player::reactions::stealPlayerCard(player, player, 0);

	EXPECT_EQ(player.getNumberOfRessources(), 1);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::BRICK), 1);
}

TEST(PlayerReactionStealAllRessources, SimpleSteal)
{
	player::Player receiver(0);
	player::Player giver(1);

	receiver.addRessource(card::RessourceType::BRICK, 2);
	giver.addRessource(card::RessourceType::BRICK, 4);
	player::reactions::stealAllRessources(receiver, giver, card::RessourceType::BRICK);

	EXPECT_EQ(receiver.getRessourceCount(card::RessourceType::BRICK), 6);
	EXPECT_EQ(giver.getRessourceCount(card::RessourceType::BRICK), 0);
}

TEST(PlayerReactionBurnCards, SimpleBurn)
{
	player::Player player(0);

	player.addRessource(card::RessourceType::BRICK, 4);
	player.addRessource(card::RessourceType::WOOL, 3);
	player.addRessource(card::RessourceType::ORE, 2);
	player.addRessource(card::RessourceType::LUMBER, 2);

	std::unordered_map<card::RessourceType, int> ressourceToBurn =
	{
		{ card::RessourceType::BRICK, 2 },
		{ card::RessourceType::WOOL, 1 },
		{ card::RessourceType::ORE, 1 }
	};

	bool success = player::reactions::burnCards(player, ressourceToBurn);

	EXPECT_TRUE(success);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::BRICK), 2);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::WOOL), 2);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::ORE), 1);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::LUMBER), 2);
}

TEST(PlayerReactionBurnCards, NotEnoughRessourcesToBurn)
{
	player::Player player(0);

	player.addRessource(card::RessourceType::BRICK, 4);
	player.addRessource(card::RessourceType::WOOL, 4);
	player.addRessource(card::RessourceType::ORE, 4);
	player.addRessource(card::RessourceType::LUMBER, 4);

	std::unordered_map<card::RessourceType, int> ressourceToBurn =
	{
		{ card::RessourceType::BRICK, 5 },
		{ card::RessourceType::WOOL, 5 },
		{ card::RessourceType::ORE, 5 }
	};

	bool success = player::reactions::burnCards(player, ressourceToBurn);

	EXPECT_FALSE(success);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::BRICK), 4);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::WOOL), 4);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::ORE), 4);
	EXPECT_EQ(player.getRessourceCount(card::RessourceType::LUMBER), 4);
}