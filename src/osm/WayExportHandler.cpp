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

    // TODO extract lanes
    auto roadId = targetNetwork.addRoad(prevNodeId.value(), currentNodeId, {1});

    prevNodeId = currentNodeId;
  }
}
WayExportHandler::WayExportHandler(
    RoadNetwork &targetNetwork,
    std::unordered_map<osmium::object_id_type, RoadNetwork::Id> const &nodeMap)
    : targetNetwork(targetNetwork), nodeMap(nodeMap) {}
} // namespace citty
