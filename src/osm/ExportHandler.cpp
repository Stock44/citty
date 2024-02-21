//
// Created by hiram on 2/20/24.
//

#include "ExportHandler.hpp"
#include "osmium/osm/node.hpp"
#include "osmium/osm/way.hpp"
#include <cmath>
#include <iostream>
#include <ranges>

namespace citty {
void ExportHandler::way(osmium::Way const &way) {
  auto prevNodeId = std::optional<NodeId>{};

  for (auto node : way.nodes()) {
    if (!nodeMap.contains(node.ref())) {
      auto nodeId = targetNetwork.addNode();

      nodeMap.try_emplace(node.ref(), nodeId);
    }

    auto currentNodeId = nodeMap.at(node.ref());

    if (!prevNodeId.has_value()) {
      prevNodeId = currentNodeId;
      continue;
    }

    // TODO extract lanes
    auto roadId = targetNetwork.addRoad(prevNodeId.value(), currentNodeId, {1});

    prevNodeId = currentNodeId;
  }
}

void ExportHandler::node(osmium::Node const &node) {
  auto location = node.location();

  auto relativeLatitude = location.lat() - centerLatitude;
  auto relativeLongitude = location.lon() - centerLongitude;

  // Assumes earth is a perfect sphere. Should be good enough for small-ish
  // areas
  constexpr double R = 6371000; // Radius of Earth in meters

  auto x =
      static_cast<float>(R * cos(relativeLatitude) * cos(relativeLongitude));
  auto y = static_cast<float>(R * sin(relativeLatitude));

  if (nodeMap.contains(node.id())) {
    auto nodeId = nodeMap[node.id()];

    targetNetwork[nodeId].position = QVector3D(x, y, 0);
  }

  auto nodeId = targetNetwork.addNode({QVector3D(x, y, 0)});

  nodeMap.try_emplace(node.id(), nodeId);
}

ExportHandler::ExportHandler(RoadNetwork &targetNetwork,
                             std::pair<double, double> latitudeBounds,
                             std::pair<double, double> longitudeBounds)
    : targetNetwork(targetNetwork), latitudeBounds(latitudeBounds),
      longitudeBounds(longitudeBounds) {
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

} // namespace citty
