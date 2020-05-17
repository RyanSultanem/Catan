#include <actions/DevelopmentAction.hpp>

#include <card/DevelopmentStock.hpp>

#include <player/Player.hpp>
#include <player/PlayerReactions.hpp>

BuyDevelopmentAction::BuyDevelopmentAction(player::Player & player, DevelopmentStock & developmentStock)
	: m_player(player)
	, m_developmentStock(developmentStock)
{
}

bool BuyDevelopmentAction::execute(board::Board & board)
{
	bool isSuccess = preExecute();

	if (isSuccess)
	{
		if (player::reactions::developmentPay(m_player))
		{
			std::optional<card::Development> optDevelopment = m_developmentStock.drawCard();
			if (optDevelopment)
			{
				m_player.receiveDevelopment(*optDevelopment);
				isSuccess = true;
			}
		}
	}

	return isSuccess;
}

ActionType BuyDevelopmentAction::getType() const
{
	return ActionType::BuyDevelopment;
}

bool BuyDevelopmentAction::preExecute()
{
	return player::reactions::developmentRessourceAvailable(m_player) && !m_developmentStock.empty();
}

UseDevelopmentAction::UseDevelopmentAction(player::Player & player, const card::DevelopmentData & developmentData, Achievement & strongestArmy)
	: m_player(player)
	, m_developmentData(developmentData)
	, m_strongestArmy(strongestArmy)
{
}

bool UseDevelopmentAction::execute(board::Board & board)
{
	std::optional<card::DevelopmentRef> optDevelopmentCard = m_player.getUnusedDevelopment(m_developmentData.getDevelopmentType());
	if (!optDevelopmentCard)
		return false;

	card::Development & developmentCard = *optDevelopmentCard;
	return developmentCard.executeAction(m_player, m_developmentData);
}

ActionType UseDevelopmentAction::getType() const
{
	return ActionType::UseDevelopment;
}

bool UseDevelopmentAction::validPrePlayerDecisionUse(bool developmentUsed) const
{
	return !developmentUsed && m_developmentData.getDevelopmentType() == card::DevelopmentType::Knight;
}

bool UseDevelopmentAction::validPlayerDecisionUse(bool developmentUsed) const
{
	if (m_developmentData.getDevelopmentType() == card::DevelopmentType::VictoryPoint)
		return true;

	return !developmentUsed;
}
