#include "gtest/gtest.h"

#include "Vertex.h"
#include "Building.hpp"
#include "Road.hpp"

using namespace board;

TEST(VertexTest, VertexCreation)
{
   Vertex v0;
   Vertex v1(1);

   ASSERT_EQ(0, v0.getId());
   ASSERT_EQ(1, v1.getId());
}

TEST(VertexTest, VertexNoBuildingConstruction)
{
   Vertex vertex;

   ASSERT_EQ(std::nullopt, vertex.getBuilding());
}

TEST(VertexTest, VertexSettlmentConstruction)
{
   Vertex vertex;

   token::building::Settlement settlment(0);
   
   vertex.setBuilding(settlment);

   ASSERT_NE(std::nullopt, vertex.getBuilding());
   ASSERT_EQ(1, vertex.getBuilding().value()->getPoints()); 
}

TEST(VertexTest, VertexCityConsutrction)
{
   Vertex vertex;

   token::building::City city(0);

   vertex.setBuilding(city);

   ASSERT_NE(std::nullopt, vertex.getBuilding());
   ASSERT_EQ(2, vertex.getBuilding().value()->getPoints());
}