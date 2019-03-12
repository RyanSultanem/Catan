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
typename std::unordered_map<Item, int>::iterator getIndexIterator(std::unordered_map<Item, int> & unorderedMap, int index)
{
	if (index > getCount(unorderedMap))
		return unorderedMap.end();

	return std::find_if(unorderedMap.begin(), unorderedMap.end(),
		[&index](const std::pair<Item, int> & element)
	{
		if (index - element.second <= 0)
			return true;

		index -= element.second;
		return false;
	});
}

} // namespace utiliy

#endif // !UTILITY_HPP