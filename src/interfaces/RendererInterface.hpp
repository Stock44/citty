//
// Created by hiram on 2/19/24.
//

#pragma once

#include <Qt3DCore>
#include <QtQmlIntegration>

namespace citty {
class RendererInterface: public QObject {
  Q_OBJECT;

  QML_ELEMENT;
  QML_UNCREATABLE("Passed from root component to OSM import dialog.");

public:
  Q_INVOKABLE void loadScene(Qt3DCore::QEntity *scene);

  void onSceneLoad(std::function<void(Qt3DCore::QEntity *)> callback);
};

} // namespace citty
