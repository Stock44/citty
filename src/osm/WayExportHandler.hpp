//
// Created by hiram on 2/22/24.
//

#pragma once

#include "roadNetwork/RoadNetwork.hpp"
#include <osmium/handler.hpp>
#include <osmium/osm/object.hpp>
namespace citty {

class WayExportHandler : public osmium::handler::Handler {
public:
  WayExportHandler(RoadNetwork &targetNetwork,
                   std::unordered_map<osmium::object_id_type,
                                      RoadNetwork::NodeId> const &nodeMap);

  void way(osmium::Way const &way);

private:
  RoadNetwork &targetNetwork;
  std::unordered_map<osmium::object_id_type, RoadNetwork::NodeId> const &nodeMap;
  const std::unordered_set<std::string> allowedHighwayTypes = {
      "motorway",
      "trunk",
      "primary",
      "secondary",
      "tertiary",
      "unclassified",
      "residential",
      "motorway_link",
      "trunk_link",
      "primary_link",
      "secondary_link",
      "tertiary_link",
      "living_street",
      "service"
  };
};

} // namespace citty
