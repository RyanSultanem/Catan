#ifndef CONDITIONS_HPP
#define CONDITIONS_HPP

#include "Vertex.fwd.h"
#include "Edge.fwd.h"

class PlaceSettlementCondition
{
public:
	explicit PlaceSettlementCondition(int playerReference);
	virtual bool checkCondition(const board::Vertex & vertex) const;

private:
	int m_playerReference;
};

class PlaceInitialSettlementCondition : public PlaceSettlementCondition
{
public:
	explicit PlaceInitialSettlementCondition(int playerReference);
	bool checkCondition(const board::Vertex & vertex) const override;
};

class PlaceRoadCondition
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

class PlaceCityCondition
{
public:
	explicit PlaceCityCondition(int playerReference);
	bool checkConditon(const board::Vertex & vertex) const;

private:
	int m_playerReference;
};

#endif // !CONDITIONS_HPP