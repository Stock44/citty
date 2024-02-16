//
// Created by hiram on 2/16/24.
//

#include "OsmMapImporter.hpp"
#include <iostream>

namespace citty {

void OsmMapImporter::import(const QGeoCoordinate &c1,
                            const QGeoCoordinate &c2) {
  std::cout << c1.latitude() << " " << c1.longitude();
  std::cout.flush();
}

} // namespace citty
