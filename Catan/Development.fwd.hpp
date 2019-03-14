#ifndef DEVELOPMENT_FWD_HPP
#define DEVELOPMENT_FWD_HPP

#include <functional>

namespace card {

class DevelopmentData;
class Development;
typedef std::reference_wrapper<Development> DevelopmentRef;
typedef std::reference_wrapper<const Development> DevelopmentCRef;
class DevelopmentAction;
enum class DevelopmentType;

} // namespace card


#endif // !DEVELOPMENT_FWD_HPP
