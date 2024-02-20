//
// Created by hiram on 2/19/24.
//

#include "SceneInterface.hpp"
#include <iostream>
#include <utility>

namespace citty {
void SceneInterface::loadScene(Qt3DCore::QEntity *scene) {
  sceneLoadCallback(scene);
}
SceneInterface::SceneInterface(
    std::function<void(Qt3DCore::QEntity *)> sceneLoadCallback)
    : sceneLoadCallback(std::move(sceneLoadCallback)) {}
} // namespace citty
