#include "gtest/gtest.h"

#include <board/Edge.hpp>
#include <board/Vertex.hpp>

#include <mock/OwnerMock.hpp>

#include <token/Road.hpp>

using namespace board;

TEST(EdgeTest, EdgeCreation)
{
   Vertex v1, v2;

   Edge edge(v1, v2);
   Edge edge2(v2, v1, 2);

   EXPECT_EQ(0, edge.getId());
   EXPECT_EQ(2, edge2.getId());
}

TEST(EdgeTest, EdgeRoadCreation)
{
   Vertex v1, v2;
   Edge edge(v1, v2); 

   token::Road road(OwnerMock(0));

   ASSERT_EQ(std::nullopt, edge.getRoad());

   edge.setRoad(road);

   EXPECT_NE(std::nullopt, edge.getRoad());
   EXPECT_EQ(road, *(edge.getRoad().value()));
}

TEST(EdgeTest, EdgeNoRoad)
{
   Vertex v1, v2;
   Edge edge(v1, v2);

   EXPECT_EQ(std::nullopt, edge.getRoad());
}