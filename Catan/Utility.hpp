#ifndef UTILITY_HPP
#define UTILITY_HPP

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

} // namespace utiliy

#endif // !UTILITY_HPP