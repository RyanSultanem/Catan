#include "Intersection.h"

Intersection::Intersection() : m_first(nullptr), m_second(nullptr), m_third(nullptr)
{
}

void Intersection::setFirstIntersection(std::shared_ptr<Intersection> intersection)
{
	m_first = intersection;
}

void Intersection::setSecondIntersection(std::shared_ptr<Intersection> intersection)
{
	m_second = intersection;
}

void Intersection::setThirdIntersection(std::shared_ptr<Intersection> intersection)
{
	m_third = intersection;
}

std::shared_ptr<Intersection> Intersection::getFirstIntersection()
{
	return m_first;
}

std::shared_ptr<Intersection> Intersection::getSecondIntersection()
{
	return m_second;
}

std::shared_ptr<Intersection> Intersection::getThirdIntersection()
{
	return m_third;
}
