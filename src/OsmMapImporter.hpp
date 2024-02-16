//
// Created by hiram on 2/16/24.
//

#pragma once

#include <QGeoCoordinate>
#include <utility>

namespace citty {

class OsmMapImporter {
public:
  void import(const QGeoCoordinate &corner1, const QGeoCoordinate &corner2);

private:
};

} // namespace citty
