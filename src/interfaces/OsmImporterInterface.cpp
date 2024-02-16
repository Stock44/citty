//
// Created by hiram on 2/16/24.
//

#include "OsmImporterInterface.hpp"

namespace citty {

OsmImporterInterface::OsmImporterInterface(OsmMapImporter &importer)
    : importer(importer) {}

void OsmImporterInterface::importOsmRegion(const QGeoCoordinate &c1,
                                           const QGeoCoordinate &c2) {
  importer.import(c1, c2);
}

} // namespace citty
