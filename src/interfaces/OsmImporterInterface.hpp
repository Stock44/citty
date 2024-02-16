//
// Created by hiram on 2/16/24.
//
#pragma once

#include "OsmMapImporter.hpp"
#include <QGeoCoordinate>
#include <QtQmlIntegration>
#include <iostream>

namespace citty {

class OsmImporterInterface : public QObject {
  Q_OBJECT;

  QML_ELEMENT;
  QML_UNCREATABLE("Passed from root component to OSM import dialog.");

public:
  explicit OsmImporterInterface(OsmMapImporter &importer);

  Q_INVOKABLE
  void importOsmRegion(const QGeoCoordinate &c1, const QGeoCoordinate &c2);

private:
  OsmMapImporter &importer;
};

} // namespace citty
