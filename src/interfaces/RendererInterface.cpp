//
// Created by hiram on 2/19/24.
//

#include "RendererInterface.hpp"

namespace citty {
void RendererInterface::onSceneLoad(
    std::function<void(Qt3DCore::QEntity *)> callback) {}
void RendererInterface::loadScene(Qt3DCore::QEntity *scene) {}
} // namespace citty
