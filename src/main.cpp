#include "OsmMapImporter.hpp"
#include "interfaces/OsmImporterInterface.hpp"
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
  const auto url = QUrl("qrc:/main/views/Citty.qml");

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);

  auto osmImporter = OsmMapImporter();

  engine.setInitialProperties(
      {{"osmImporter",
        QVariant::fromValue(new citty::OsmImporterInterface(osmImporter))}});

  engine.load(url);

  return QGuiApplication::exec();
}
