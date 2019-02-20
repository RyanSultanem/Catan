#ifndef CONDITIONS_HPP
#define CONDITIONS_HPP

#include "Vertex.fwd.h"
#include "Edge.fwd.h"

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
	explicit PlaceInitialSettlementCondition(int playerReference);
	bool checkCondition(const board::Vertex & vertex) const override;
};

class PlaceRoadCondition : public PlaceTokenCondition<board::Edge>
{
public:
	explicit PlaceRoadCondition(int playerReference);
	virtual bool checkCondition(const board::Edge & edge) const;

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
	bool checkCondition(const board::Vertex & vertex) const;

private:
	int m_playerReference;
};

#endif // !CONDITIONS_HPP