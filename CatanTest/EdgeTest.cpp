#include "gtest/gtest.h"

#include "Edge.h"
#include "Vertex.h"
#include "Road.hpp"

using namespace board;

class OwnerMock : public player::Owner
{
public:
   int getId() const override { return 0; }
};

static OwnerMock ownerMock;

TEST(EdgeTest, EdgeCreation)
{
   Vertex v1, v2;

   Edge edge(v1, v2);
   Edge edge2(v2, v1, 2);

   ASSERT_EQ(0, edge.getId());
   ASSERT_EQ(2, edge2.getId());
}

TEST(EdgeTest, EdgeRoadCreation)
{
   Vertex v1, v2;
   Edge edge(v1, v2); 

   token::Road road(ownerMock);

   ASSERT_EQ(std::nullopt, edge.getRoad());

   edge.setRoad(road);

   ASSERT_NE(std::nullopt, edge.getRoad());
   ASSERT_EQ(road, *(edge.getRoad().value()));
}

TEST(EdgeTest, EdgeNoRoad)
{
   Vertex v1, v2;
   Edge edge(v1, v2);

   ASSERT_EQ(std::nullopt, edge.getRoad());
}