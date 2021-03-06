#ifndef MOCK_PLACECONDICITONMOCK_HPP
#define MOCK_PLACECONDICITONMOCK_HPP

#include <token/Conditions.hpp>

class PlaceRoadConditionMock : public PlaceRoadCondition
{
public:
	explicit PlaceRoadConditionMock(int playerReference)
		: PlaceRoadCondition(playerReference)
	{
	}
	bool checkCondition(const board::Edge & edge) const override
	{
		return true;
	};
};

#endif // !MOCK_PLACECONDICITONMOCK_HPP