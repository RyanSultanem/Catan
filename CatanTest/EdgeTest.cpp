#include "gtest/gtest.h"

#include <board/Edge.hpp>
#include <board/Vertex.hpp>

#include <token/Road.hpp>

using namespace board;

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

   token::Road road(0);

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