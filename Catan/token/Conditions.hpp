#ifndef TOKEN_CONDITIONS_CPP
#define TOKEN_CONDITIONS_CPP

#include <board/Edge.fwd.hpp>
#include <board/Vertex.fwd.hpp>

// Not sure if useful
template<typename Location>
class PlaceTokenCondition
{
public:
	virtual bool checkCondition(const Location & location) const = 0;
};

class PlaceSettlementCondition : public PlaceTokenCondition<board::Vertex>
{
public:
	explicit PlaceSettlementCondition(int playerReference);
	bool checkCondition(const board::Vertex & vertex) const;

private:
	int m_playerReference;
};

class PlaceInitialSettlementCondition : public PlaceSettlementCondition
{
public:
	explicit PlaceInitialSettlementCondition(int playerReference, int initialRoadPosition);
	bool checkCondition(const board::Vertex & vertex) const override;

private:
	int m_initialRoadPosition;
};

class PlaceRoadCondition : public PlaceTokenCondition<board::Edge>
{
public:
	explicit PlaceRoadCondition(int playerReference);
	bool checkCondition(const board::Edge & edge) const override;

private:
	int m_playerReference;
};

class PlaceInitialRoadCondition : public PlaceRoadCondition
{
public:
	explicit PlaceInitialRoadCondition(int playerReference, int initialSettlementPosition);
	bool checkCondition(const board::Edge & edge) const override;

private:
	int m_initialSettlementPosition;
};

class PlaceCityCondition : public PlaceTokenCondition<board::Vertex>
{
public:
	explicit PlaceCityCondition(int playerReference);
	bool checkCondition(const board::Vertex & vertex) const override;

private:
	int m_playerReference;
};

#endif // !TOKEN_CONDITION_FWD_CPP