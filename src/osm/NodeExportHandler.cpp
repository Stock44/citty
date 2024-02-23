//
// Created by hiram on 2/20/24.
//

#include "NodeExportHandler.hpp"
#include <cmath>
#include <osmium/osm/node.hpp>
#include <osmium/osm/way.hpp>
#include <ranges>

namespace citty {

void NodeExportHandler::node(osmium::Node const &node) {
  auto location = node.location();

  auto x = longitudeDistance(centerLatitude, location.lon(), centerLongitude);
  auto z = latitudeDistance(location.lat(), centerLatitude);

  auto nodeId = targetNetwork.addNode(
      {QVector3D(location.lon() > centerLongitude ? x : -x, 0,
                 location.lat() > centerLatitude ? z : -z)});
  auto [it, ok] = nodeMap.try_emplace(node.id(), nodeId);

  if (!ok) {
    throw std::runtime_error("attempted to add a node that already exists");
  }
}

NodeExportHandler::NodeExportHandler(RoadNetwork &targetNetwork,
                                     std::pair<double, double> latitudeBounds,
                                     std::pair<double, double> longitudeBounds)
    : targetNetwork(targetNetwork) {
  if (latitudeBounds.first > latitudeBounds.second ||
      longitudeBounds.first > longitudeBounds.second) {
    throw std::runtime_error(
        "first element of range must be less than the second");
  }

  latitudeRange = latitudeBounds.second - latitudeBounds.first;
  longitudeRange = longitudeBounds.second - longitudeBounds.first;

  centerLatitude = latitudeBounds.first + latitudeRange / 2.0;
  centerLongitude = longitudeBounds.first + longitudeRange / 2.0;
}
const std::unordered_map<osmium::object_id_type, RoadNetwork::Id> &
NodeExportHandler::getNodeMap() const {
  return nodeMap;
}

double latitudeDistance(double lat1, double lat2) {
  const double R = 6371000;

  double dLat = degToRad(lat2 - lat1);

  double a = std::pow(std::sin(dLat / 2.0), 2);

  double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

  return R * c;
}

double longitudeDistance(double lat, double lon1, double lon2) {
  const double R = 6371000;

  double dLon = degToRad(lon2 - lon1);

  double a = std::pow(std::cos(degToRad(lat)), 2.0) *
             std::pow(std::sin(dLon / 2.0), 2.0);

  double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
  return R * c;
}

} // namespace citty
