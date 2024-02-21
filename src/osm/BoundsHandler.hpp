//
// Created by hiram on 2/20/24.
//

#pragma once

#include "osmium/handler.hpp"
#include <limits>
#include <utility>
namespace citty {

class BoundsHandler : public osmium::handler::Handler {
public:
  void node(osmium::Node const &node);

  [[nodiscard]] double maxLatitude() const;
  [[nodiscard]] double minLatitude() const;
  [[nodiscard]] double maxLongitude() const;
  [[nodiscard]] double minLongitude() const;

private:
  double _maxLatitude = std::numeric_limits<double>::lowest();
  double _minLatitude = std::numeric_limits<double>::max();
  double _maxLongitude = std::numeric_limits<double>::lowest();
  double _minLongitude = std::numeric_limits<double>::max();
};

} // namespace citty
