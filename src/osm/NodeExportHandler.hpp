//
// Created by hiram on 2/20/24.
//

#pragma once

#include "roadNetwork/RoadNetwork.hpp"
#include <osmium/handler.hpp>
#include <osmium/osm/object.hpp>

namespace citty {

inline double degToRad(double deg) { return deg * std::numbers::pi / 180.0; }
double latitudeDistance(double lat1, double lat2);
double longitudeDistance(double lat, double lon1, double lon2);

class NodeExportHandler : public osmium::handler::Handler {
public:
  NodeExportHandler(RoadNetwork &targetNetwork,
                std::pair<double, double> latitudeBounds,
                std::pair<double, double> longitudeBounds);


  void node(osmium::Node const &node);

  const std::unordered_map<osmium::object_id_type, RoadNetwork::NodeId> &
  getNodeMap() const;

private:
  RoadNetwork &targetNetwork;
  std::unordered_map<osmium::object_id_type, RoadNetwork::NodeId> nodeMap;

  double centerLatitude;
  double centerLongitude;
  double latitudeRange;
  double longitudeRange;
};
} // namespace citty
