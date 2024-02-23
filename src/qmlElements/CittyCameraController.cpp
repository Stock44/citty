//
// Created by hiram on 2/22/24.
//

#include "CittyCameraController.hpp"
#include <QCamera>

namespace citty {
void CittyCameraController::moveCamera(const InputState &state, float dt) {
  Qt3DRender::QCamera *theCamera = camera();
  if (theCamera == nullptr)
    return;

  auto mult = state.shiftKeyActive ? 5.0 : 1.0;

  theCamera->translate(QVector3D(state.txAxisValue * linearSpeed() * mult,
                                 state.tyAxisValue * linearSpeed() * mult,
                                 state.tzAxisValue * linearSpeed() * mult) *
                       dt);
  if (state.leftMouseButtonActive) {
    float theLookSpeed = lookSpeed();

    const QVector3D upVector(0.0f, 1.0f, 0.0f);
    theCamera->pan(state.rxAxisValue * theLookSpeed * dt, upVector);
    theCamera->tilt(state.ryAxisValue * theLookSpeed * dt);
  }
}

CittyCameraController::CittyCameraController(Qt3DCore::QNode *parent)
    : Qt3DCore::QEntity(parent) {
  // Left Mouse Button Action
  leftMouseButtonInput->setButtons(QVector<int>() << Qt::LeftButton);
  leftMouseButtonInput->setSourceDevice(mouseDevice);
  leftMouseButtonAction->addInput(leftMouseButtonInput);
  // Middle Mouse Button Action
  middleMouseButtonInput->setButtons(QVector<int>() << Qt::MiddleButton);
  middleMouseButtonInput->setSourceDevice(mouseDevice);
  middleMouseButtonAction->addInput(middleMouseButtonInput);
  // Right Mouse Button Action
  rightMouseButtonInput->setButtons(QVector<int>() << Qt::RightButton);
  rightMouseButtonInput->setSourceDevice(mouseDevice);
  rightMouseButtonAction->addInput(rightMouseButtonInput);
  // Alt Button Action
  altButtonInput->setButtons(QVector<int>() << Qt::Key_Alt);
  altButtonInput->setSourceDevice(keyboardDevice);
  altButtonAction->addInput(altButtonInput);
  // Shift Button Action
  shiftButtonInput->setButtons(QVector<int>() << Qt::Key_Shift);
  shiftButtonInput->setSourceDevice(keyboardDevice);
  shiftButtonAction->addInput(shiftButtonInput);
  // Escape Button Action
  escapeButtonInput->setButtons(QVector<int>() << Qt::Key_Escape);
  escapeButtonInput->setSourceDevice(keyboardDevice);
  escapeButtonAction->addInput(escapeButtonInput);
  //// Axes
  // Mouse X
  mouseRxInput->setAxis(Qt3DInput::QMouseDevice::X);
  mouseRxInput->setSourceDevice(mouseDevice);
  rxAxis->addInput(mouseRxInput);
  // Mouse Y
  mouseRyInput->setAxis(Qt3DInput::QMouseDevice::Y);
  mouseRyInput->setSourceDevice(mouseDevice);
  ryAxis->addInput(mouseRyInput);
  // Mouse Wheel X
  mouseTzXInput->setAxis(Qt3DInput::QMouseDevice::WheelX);
  mouseTzXInput->setSourceDevice(mouseDevice);
  tzAxis->addInput(mouseTzXInput);
  // Mouse Wheel Y
  mouseTzYInput->setAxis(Qt3DInput::QMouseDevice::WheelY);
  mouseTzYInput->setSourceDevice(mouseDevice);
  tzAxis->addInput(mouseTzYInput);
  // Keyboard Pos Tx
  keyboardTxPosInput->setButtons(QVector<int>() << Qt::Key_Right << Qt::Key_D);
  keyboardTxPosInput->setScale(1.0f);
  keyboardTxPosInput->setSourceDevice(keyboardDevice);
  txAxis->addInput(keyboardTxPosInput);
  // Keyboard Pos Tz
  keyboardTzPosInput->setButtons(QVector<int>() << Qt::Key_Up << Qt::Key_W);
  keyboardTzPosInput->setScale(1.0f);
  keyboardTzPosInput->setSourceDevice(keyboardDevice);
  tzAxis->addInput(keyboardTzPosInput);
  // Keyboard Pos Ty
  keyboardTyPosInput->setButtons(QVector<int>() << Qt::Key_PageUp << Qt::Key_E);
  keyboardTyPosInput->setScale(1.0f);
  keyboardTyPosInput->setSourceDevice(keyboardDevice);
  tyAxis->addInput(keyboardTyPosInput);
  // Keyboard Neg Tx
  keyboardTxNegInput->setButtons(QVector<int>() << Qt::Key_Left << Qt::Key_A);
  keyboardTxNegInput->setScale(-1.0f);
  keyboardTxNegInput->setSourceDevice(keyboardDevice);
  txAxis->addInput(keyboardTxNegInput);
  // Keyboard Neg Tz
  keyboardTzNegInput->setButtons(QVector<int>() << Qt::Key_Down << Qt::Key_S);
  keyboardTzNegInput->setScale(-1.0f);
  keyboardTzNegInput->setSourceDevice(keyboardDevice);
  tzAxis->addInput(keyboardTzNegInput);
  // Keyboard Neg Ty
  keyboardTyNegInput->setButtons(QVector<int>()
                                 << Qt::Key_PageDown << Qt::Key_Q);
  keyboardTyNegInput->setScale(-1.0f);
  keyboardTyNegInput->setSourceDevice(keyboardDevice);
  tyAxis->addInput(keyboardTyNegInput);
  //// Logical Device
  logicalDevice->addAction(leftMouseButtonAction);
  logicalDevice->addAction(middleMouseButtonAction);
  logicalDevice->addAction(rightMouseButtonAction);
  logicalDevice->addAction(altButtonAction);
  logicalDevice->addAction(shiftButtonAction);
  logicalDevice->addAction(escapeButtonAction);
  logicalDevice->addAxis(rxAxis);
  logicalDevice->addAxis(ryAxis);
  logicalDevice->addAxis(txAxis);
  logicalDevice->addAxis(tyAxis);
  logicalDevice->addAxis(tzAxis);

  // Disable the logical device when the entity is disabled
  QObject::connect(this, &Qt3DCore::QEntity::enabledChanged, logicalDevice,
                   &Qt3DInput::QLogicalDevice::setEnabled);
  QObject::connect(this, &Qt3DCore::QEntity::enabledChanged, frameAction,
                   &Qt3DLogic::QFrameAction::setEnabled);
  for (auto axis : {rxAxis, ryAxis, txAxis, tyAxis, tzAxis}) {
    QObject::connect(this, &Qt3DCore::QEntity::enabledChanged, axis,
                     &Qt3DInput::QAxis::setEnabled);
  }
  QObject::connect(escapeButtonAction, &Qt3DInput::QAction::activeChanged, this,
                   [this](bool isActive) {
                     if (isActive && _camera)
                       _camera->viewAll();
                   });
  addComponent(frameAction);
  addComponent(logicalDevice);

  QObject::connect(
      frameAction, &Qt3DLogic::QFrameAction::triggered, this, [this](float dt) {
        InputState state{};
        state.rxAxisValue = rxAxis->value();
        state.ryAxisValue = ryAxis->value();
        state.txAxisValue = txAxis->value();
        state.tyAxisValue = tyAxis->value();
        state.tzAxisValue = tzAxis->value();
        state.leftMouseButtonActive = leftMouseButtonAction->isActive();
        state.middleMouseButtonActive = middleMouseButtonAction->isActive();
        state.rightMouseButtonActive = rightMouseButtonAction->isActive();
        state.altKeyActive = altButtonAction->isActive();
        state.shiftKeyActive = shiftButtonAction->isActive();
        moveCamera(state, dt);
      });
}
Qt3DRender::QCamera *CittyCameraController::camera() const { return _camera; }
float CittyCameraController::linearSpeed() const { return _linearSpeed; }
float CittyCameraController::lookSpeed() const { return _lookSpeed; }
void CittyCameraController::setCamera(Qt3DRender::QCamera *camera) {
  if (_camera != camera) {
    _camera = camera;
    emit cameraChanged();
  }
}
void CittyCameraController::setLinearSpeed(float linearSpeed) {
  if (_linearSpeed != linearSpeed) {
    _linearSpeed = linearSpeed;
    emit linearSpeedChanged();
  }
}
void CittyCameraController::setLookSpeed(float lookSpeed) {
  if (_lookSpeed != lookSpeed) {
    _lookSpeed = lookSpeed;
    emit lookSpeedChanged();
  }
}
} // namespace citty
