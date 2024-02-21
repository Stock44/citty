#include "RoadNetwork.hpp"
#include "SceneInterface.hpp"
#include "interfaces/OsmImporterInterface.hpp"
#include "osm/OsmRoadNetworkImporter.hpp"
#include <QDirIterator>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtPlugin>
#include <QtQml>
#include <iostream>

int main(int argc, char *argv[]) {
  using namespace citty;

  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  const auto url =
      QUrl("qrc:/github.com/Stock44/imports/citty/views/Citty.qml");

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);

  auto roadNetwork = RoadNetwork();

  auto osmImporter = OsmRoadNetworkImporter(roadNetwork);
  auto sceneInterface = new SceneInterface(
      [](auto scene) { std::cout << "Loading scene" << std::endl; });

  engine.setInitialProperties(
      {{
           "osmImporterInterface",
           QVariant::fromValue(new citty::OsmImporterInterface(osmImporter)),
       },
       {"sceneInterface", QVariant::fromValue(sceneInterface)}});

  engine.load(url);

  return QGuiApplication::exec();
}
