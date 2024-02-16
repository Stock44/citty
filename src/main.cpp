#include <QtPlugin>
#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QDirIterator>
#include <iostream>
#include <QtQml>
#include "interfaces/OsmImporterInterface.hpp"


int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const auto url = QUrl("qrc:/main/views/Citty.qml");

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            }, Qt::QueuedConnection);

    engine.setInitialProperties({
                                        {"osmImporter", QVariant::fromValue(new citty::OsmImporterInterface)}
                                });

    engine.load(url);

    return QGuiApplication::exec();
}
