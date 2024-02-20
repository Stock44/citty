//
// Created by hiram on 2/19/24.
//

#pragma once

#include <Qt3DCore>
#include <QtQmlIntegration>

namespace citty {
class SceneInterface : public QObject {
  Q_OBJECT;

  QML_ELEMENT;
  QML_UNCREATABLE("Passed from root component to OSM import dialog.");

public:
  explicit SceneInterface(
      std::function<void(Qt3DCore::QEntity *)> sceneLoadCallback);

  Q_INVOKABLE void loadScene(Qt3DCore::QEntity *scene);

private:
  std::function<void(Qt3DCore::QEntity *)> sceneLoadCallback;
};

} // namespace citty
