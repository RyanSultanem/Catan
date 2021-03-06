#ifndef TOKEN_BUILDING_FWD_HPP
#define TOKEN_BUILDING_FWD_HPP
#include <functional>

namespace token {
namespace building {

class Building;
typedef std::reference_wrapper<Building> BuildingRef;
class Settlement;
typedef std::reference_wrapper<Settlement> SettlementRef;
class City;
typedef std::reference_wrapper<City> CityRef;

} // namespace building
} // namespace token

#endif // !TOKEN_BUILDING_FWD_HPP
