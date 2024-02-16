//
// Created by hiram on 2/16/24.
//

#include "OsmImporterInterface.hpp"

namespace citty {
    void OsmImporterInterface::importOsmRegion(const QGeoCoordinate &c1, const QGeoCoordinate &c2) {
        std::cout << c1.latitude() << " " << c1.longitude() << " " << c2.latitude() << " " << c2.longitude();
        std::cout.flush();
    }
} // citty
