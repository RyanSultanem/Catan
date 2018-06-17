#ifndef SERIALIZE_HPP
#define SERIALIZE_HPP

#include <string>
#include <vector>

namespace serialize {

class Serializable
{
public:
   virtual std::string serialize() const = 0;
};


template<class SerializableItem>
std::string containerSerialize(const std::vector<SerializableItem> & input, const std::string & initalValue, const std::string & tag)
{  
   std::vector<std::string> vecResult;
   std::transform(input.begin(), input.end(), std::back_inserter(vecResult), [](const Serializable & item) {
      return item.serialize();
   });

   std::string result = std::accumulate(std::next(vecResult.begin()), vecResult.end(), initalValue + tag + vecResult[0], [](const std::string & res, const std::string & element) {
      return res + " - " + element;
   });

   return std::move(result + '\n');
}

}

#endif // !SERIALIZE_HPP