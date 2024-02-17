//
// Created by hiram on 2/16/24.
//

#include "OsmMapImporter.hpp"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <iostream>
#include <osmium/io/xml_input.hpp>

namespace citty {

void OsmMapImporter::import(const QGeoCoordinate &c1,
                            const QGeoCoordinate &c2) {
  auto urlString = std::format(
      "http://overpass-api.de/api/"
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

void OsmMapImporter::receiveOsmXml(const std::string &osmXml) {
  qDebug() << osmXml;
  auto file = osmium::io::File(osmXml.c_str(), osmXml.size(), "xml");
  auto reader = osmium::io::Reader(file);

  reader.close();
}

} // namespace citty
