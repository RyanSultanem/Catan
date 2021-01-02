#ifndef MOCK_OWNERMOCK_HPP
#define MOCK_OWNERMOCK_HPP

#include <player/PlayerInterface.hpp>

class OwnerMock : public player::Owner
{
public:
	explicit OwnerMock(int id)
		: m_id(id)
	{}

	int getId() const override
	{
		return m_id;
	};

private:
	int m_id;
};

#endif // !MOCK_OWNERMOCK_HPP