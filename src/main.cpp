#include "OsmMapImporter.hpp"
#include "RendererInterface.hpp"
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
  const auto url = QUrl("qrc:/github.com/Stock44/imports/citty/views/Citty.qml");

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);

  auto osmImporter = OsmMapImporter();
  auto rendererInterface = new RendererInterface();

  engine.setInitialProperties(
      {{
           "osmImporterInterface",
           QVariant::fromValue(new citty::OsmImporterInterface(osmImporter)),
       },
       {"rendererInterface", QVariant::fromValue(rendererInterface)}});

  engine.load(url);

  return QGuiApplication::exec();
}
