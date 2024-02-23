//
// Created by hiram on 2/16/24.
//

#include "OsmRoadNetworkImporter.hpp"
#include "osm/BoundsHandler.hpp"
#include "osm/NodeExportHandler.hpp"
#include "osm/WayExportHandler.hpp"
#include "osmium/io/xml_input.hpp"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <iostream>

namespace citty {

OsmRoadNetworkImporter::OsmRoadNetworkImporter(RoadNetwork &targetNetwork)
    : targetNetwork(targetNetwork) {}

void OsmRoadNetworkImporter::import(const QGeoCoordinate &c1,
                                    const QGeoCoordinate &c2) {
  auto urlString =
      std::format("http://overpass-api.de/api/"
                  "interpreter?data=[out:xml];(node({},{},{},{});<;);out meta;",
                  c2.latitude(), c1.longitude(), c1.latitude(), c2.longitude());

  QNetworkRequest request;
  auto url = QUrl(urlString.c_str());
  qDebug() << url.errorString();
  request.setUrl(url);

  auto manager = new QNetworkAccessManager();

  QObject::connect(manager, &QNetworkAccessManager::finished,
                   [this](auto reply) {
                     if (reply->error()) {
                       qDebug() << reply->errorString();
                       return;
                     }

                     QString answer = reply->readAll();

                     receiveOsmXml(answer.toStdString());
                   });

  manager->get(request);
}

void OsmRoadNetworkImporter::receiveOsmXml(std::string const &osmXml) {
  auto file = osmium::io::File(osmXml.c_str(), osmXml.size(), "xml");
  auto reader = osmium::io::Reader(file);

  auto boundsHandler = BoundsHandler();

  osmium::apply(reader, boundsHandler);
  reader.close();

  auto nodeExportHandler = NodeExportHandler(
      targetNetwork, {boundsHandler.minLatitude(), boundsHandler.maxLatitude()},
      {boundsHandler.minLongitude(), boundsHandler.maxLongitude()});

  auto reader2 = osmium::io::Reader(file);
  osmium::apply(reader2, nodeExportHandler);
  reader2.close();

  auto wayExportHandler =
      WayExportHandler(targetNetwork, nodeExportHandler.getNodeMap());
  auto reader3 = osmium::io::Reader(file);
  osmium::apply(reader3, wayExportHandler);
  reader3.close();
}
} // namespace citty
