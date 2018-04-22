#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <memory>

#include "Intersection.fwd.h"

class Intersection
{
public:
	Intersection();
	void setFirstIntersection(std::shared_ptr<Intersection> intersection);
	void setSecondIntersection(std::shared_ptr<Intersection> intersection);
	void setThirdIntersection(std::shared_ptr<Intersection> intersection);

	std::shared_ptr<Intersection> getFirstIntersection();
	std::shared_ptr<Intersection> getSecondIntersection();
	std::shared_ptr<Intersection> getThirdIntersection();

private:
	std::shared_ptr<Intersection> m_first;
	std::shared_ptr<Intersection> m_second;
	std::shared_ptr<Intersection> m_third;
};

#endif // !INTERSECTION_H
