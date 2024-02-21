//
// Created by hiram on 2/16/24.
//

#pragma once

#include "RoadNetwork.hpp"
#include "osmium/relations/relations_manager.hpp"
#include <QGeoCoordinate>
#include <utility>

namespace citty {


class OsmRoadNetworkImporter {
public:
  explicit OsmRoadNetworkImporter(RoadNetwork &targetNetwork);

  void import(const QGeoCoordinate &corner1, const QGeoCoordinate &corner2);

  void receiveOsmXml(std::string const &osmXml);

private:
  RoadNetwork &targetNetwork;
};

} // namespace citty
