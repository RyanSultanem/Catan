#ifndef UTILITY_SERIALIZE_HPP
#define UTILITY_SERIALIZE_HPP

#include <algorithm>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

namespace serialize {

class Serializable
{
public:
   virtual std::string serialize() const = 0;
};


template<class SerializableItem>
std::string containerSerialize(const std::vector<SerializableItem> & input, const std::string & initalValue, const std::string & tag, const std::string & separator = " - ")
{  
	if (input.empty())
		return std::string();

	std::vector<std::string> serializedResults;
	std::transform(input.begin(), input.end(), std::back_inserter(serializedResults),
		[](const Serializable & item) 
	{
		return item.serialize();
	});

	std::string result = std::accumulate(std::next(serializedResults.begin()), serializedResults.end(), initalValue + tag + serializedResults[0],
		[&separator](const std::string & res, const std::string & element)
	{
		return res + separator + element;
	});

	return std::move(result + '\n');
}

}

#endif // !UTILITY_SERIALIZE_HPP