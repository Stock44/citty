//
// Created by hiram on 2/20/24.
//

#pragma once

#include "RoadNetwork.hpp"
#include <osmium/handler.hpp>
#include <osmium/osm/object.hpp>

namespace citty {

class ExportHandler : public osmium::handler::Handler {
public:
  ExportHandler(RoadNetwork &targetNetwork,
                std::pair<double, double> latitudeBounds,
                std::pair<double, double> longitudeBounds);

  void way(osmium::Way const &way);

  void node(osmium::Node const &node);

private:
  RoadNetwork &targetNetwork;
  std::unordered_map<osmium::object_id_type, NodeId> nodeMap;

  std::pair<double, double> latitudeBounds;
  std::pair<double, double> longitudeBounds;
  double centerLatitude;
  double centerLongitude;
  double latitudeRange;
  double longitudeRange;
};
} // namespace citty
