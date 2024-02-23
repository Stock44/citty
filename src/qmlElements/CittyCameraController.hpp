//
// Created by hiram on 2/22/24.
//

#pragma once
#include <QAbstractCameraController>
#include <QCamera>
#include <Qt3DInput/QAction>
#include <Qt3DInput/QActionInput>
#include <Qt3DInput/QAnalogAxisInput>
#include <Qt3DInput/QAxis>
#include <Qt3DInput/QButtonAxisInput>
#include <Qt3DInput/QKeyboardDevice>
#include <Qt3DInput/QLogicalDevice>
#include <Qt3DInput/QMouseDevice>
#include <Qt3DLogic/QFrameAction>
#include <QtQmlIntegration>

namespace citty {

class CittyCameraController : public Qt3DCore::QEntity {
  Q_OBJECT
  QML_ELEMENT
  Q_PROPERTY(Qt3DRender::QCamera *camera READ camera WRITE setCamera NOTIFY
                 cameraChanged)
  Q_PROPERTY(float linearSpeed READ linearSpeed WRITE setLinearSpeed NOTIFY
                 linearSpeedChanged)
  Q_PROPERTY(
      float lookSpeed READ lookSpeed WRITE setLookSpeed NOTIFY lookSpeedChanged)
public:
  explicit CittyCameraController(Qt3DCore::QNode *parent = nullptr);

  Qt3DRender::QCamera *camera() const;
  float linearSpeed() const;
  float lookSpeed() const;
  void setCamera(Qt3DRender::QCamera *camera);
  void setLinearSpeed(float linearSpeed);
  void setLookSpeed(float lookSpeed);

signals:
  void cameraChanged();
  void linearSpeedChanged();
  void lookSpeedChanged();
  void accelerationChanged(float acceleration);
  void decelerationChanged(float deceleration);

private:
  struct InputState {
    float rxAxisValue;
    float ryAxisValue;
    float txAxisValue;
    float tyAxisValue;
    float tzAxisValue;
    bool leftMouseButtonActive;
    bool middleMouseButtonActive;
    bool rightMouseButtonActive;
    bool altKeyActive;
    bool shiftKeyActive;
  };

  void moveCamera(const InputState &state, float dt);

  Qt3DRender::QCamera *_camera = nullptr;
  Qt3DInput::QAction *leftMouseButtonAction = new Qt3DInput::QAction();
  Qt3DInput::QAction *middleMouseButtonAction = new Qt3DInput::QAction();
  Qt3DInput::QAction *rightMouseButtonAction = new Qt3DInput::QAction();
  Qt3DInput::QAction *altButtonAction = new Qt3DInput::QAction();
  Qt3DInput::QAction *shiftButtonAction = new Qt3DInput::QAction();
  Qt3DInput::QAction *escapeButtonAction = new Qt3DInput::QAction();
  Qt3DInput::QAxis *rxAxis = new Qt3DInput::QAxis();
  Qt3DInput::QAxis *ryAxis = new Qt3DInput::QAxis();
  Qt3DInput::QAxis *txAxis = new Qt3DInput::QAxis();
  Qt3DInput::QAxis *tyAxis = new Qt3DInput::QAxis();
  Qt3DInput::QAxis *tzAxis = new Qt3DInput::QAxis();
  Qt3DInput::QActionInput *leftMouseButtonInput = new Qt3DInput::QActionInput();
  Qt3DInput::QActionInput *middleMouseButtonInput =
      new Qt3DInput::QActionInput();
  Qt3DInput::QActionInput *rightMouseButtonInput =
      new Qt3DInput::QActionInput();
  Qt3DInput::QActionInput *altButtonInput = new Qt3DInput::QActionInput();
  Qt3DInput::QActionInput *shiftButtonInput = new Qt3DInput::QActionInput();
  Qt3DInput::QActionInput *escapeButtonInput = new Qt3DInput::QActionInput();
  Qt3DInput::QAnalogAxisInput *mouseRxInput = new Qt3DInput::QAnalogAxisInput();
  Qt3DInput::QAnalogAxisInput *mouseRyInput = new Qt3DInput::QAnalogAxisInput();
  Qt3DInput::QAnalogAxisInput *mouseTzXInput =
      new Qt3DInput::QAnalogAxisInput();
  Qt3DInput::QAnalogAxisInput *mouseTzYInput =
      new Qt3DInput::QAnalogAxisInput();
  Qt3DInput::QButtonAxisInput *keyboardTxPosInput =
      new Qt3DInput::QButtonAxisInput();
  Qt3DInput::QButtonAxisInput *keyboardTyPosInput =
      new Qt3DInput::QButtonAxisInput();
  Qt3DInput::QButtonAxisInput *keyboardTzPosInput =
      new Qt3DInput::QButtonAxisInput();
  Qt3DInput::QButtonAxisInput *keyboardTxNegInput =
      new Qt3DInput::QButtonAxisInput();
  Qt3DInput::QButtonAxisInput *keyboardTyNegInput =
      new Qt3DInput::QButtonAxisInput();
  Qt3DInput::QButtonAxisInput *keyboardTzNegInput =
      new Qt3DInput::QButtonAxisInput();
  Qt3DInput::QKeyboardDevice *keyboardDevice = new Qt3DInput::QKeyboardDevice();
  Qt3DInput::QMouseDevice *mouseDevice = new Qt3DInput::QMouseDevice();
  Qt3DInput::QLogicalDevice *logicalDevice = new Qt3DInput::QLogicalDevice();
  Qt3DLogic::QFrameAction *frameAction = new Qt3DLogic::QFrameAction();
  float _linearSpeed = 1.0;
  float _lookSpeed = 180.0;
  QVector3D sceneUp = QVector3D{0.0f, 1.0f, 0.0f};
};

} // namespace citty
