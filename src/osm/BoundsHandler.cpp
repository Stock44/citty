//
// Created by hiram on 2/20/24.
//

#include "BoundsHandler.hpp"
#include "osmium/osm/node.hpp"

namespace citty {
double BoundsHandler::maxLatitude() const { return _maxLatitude; }
double BoundsHandler::minLatitude() const { return _minLatitude; }
double BoundsHandler::maxLongitude() const { return _maxLongitude; }
double BoundsHandler::minLongitude() const { return _minLongitude; }

void BoundsHandler::node(const osmium::Node &node) {
  auto location = node.location();
  auto latitude = location.lat();
  auto longitude = location.lon();

  if (latitude > _maxLatitude) {
    _maxLatitude = latitude;
  } else if (latitude < _minLatitude) {
    _minLatitude = latitude;
  }

  if (longitude > _maxLongitude) {
    _maxLongitude = longitude;
  } else if (longitude < _minLongitude) {
    _minLongitude = longitude;
  }
}

} // namespace citty
