#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <numeric>
#include <algorithm>
#include <unordered_map>

namespace utility {

template<typename Item>
int getCount(const std::unordered_map<Item, int> & unorderedMap)
{
	return std::accumulate(unorderedMap.begin(), unorderedMap.end(), 0,
		[](int sum, const auto & element) {
		return sum + element.second;
	});
}

template<typename Item>
typename std::unordered_map<Item, int>::iterator getRandomIterator(std::unordered_map<Item, int> & unorderedMap)
{
	int randomIndex = (rand() % getCount(unorderedMap)) + 1;

	auto it = std::find_if(unorderedMap.begin(), unorderedMap.end(),
		[&randomIndex](const std::pair<Item, int> & element)
	{
		if (randomIndex - element.second <= 0)
			return true;

		randomIndex -= element.second;
		return false;
	});

	return it;
}

} // namespace utiliy

#endif // !UTILITY_HPP