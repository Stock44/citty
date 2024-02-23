//
// Created by hiram on 2/22/24.
//

#include "WayExportHandler.hpp"
#include <osmium/osm/way.hpp>

namespace citty {
void WayExportHandler::way(osmium::Way const &way) {
  if (!way.tags().has_key("highway")) {
    return;
  }

  if (!allowedHighwayTypes.contains(way.tags().get_value_by_key("highway"))) {
    return;
  }
  auto prevNodeId = std::optional<RoadNetwork::Id>{};

  for (auto node : way.nodes()) {
    if (!nodeMap.contains(node.ref()))
      continue;
    auto currentNodeId = nodeMap.at(node.ref());

    if (!prevNodeId.has_value()) {
      prevNodeId = currentNodeId;
      continue;
    }

    auto lanes = 1;

    if (way.tags().has_key("lanes:forward")) {
      lanes = std::stoi(way.tags().get_value_by_key("lanes:forward"));
    } else if (way.tags().has_key("lanes")) {
      lanes = std::stoi(way.tags().get_value_by_key("lanes"));
    }

    targetNetwork.addRoad(prevNodeId.value(), currentNodeId, {lanes});

    if (std::strcmp(way.tags().get_value_by_key("oneway", "yes"), "yes") != 0) {
      auto reverseLanes = 1;
      if (way.tags().has_key("lanes:backward")) {
        reverseLanes = std::stoi(way.tags().get_value_by_key("lanes:backward"));
      }
      targetNetwork.addRoad(currentNodeId, prevNodeId.value(), {reverseLanes});
    }

    prevNodeId = currentNodeId;
  }
}
WayExportHandler::WayExportHandler(
    RoadNetwork &targetNetwork,
    std::unordered_map<osmium::object_id_type, RoadNetwork::Id> const &nodeMap)
    : targetNetwork(targetNetwork), nodeMap(nodeMap) {}
} // namespace citty
