import citty
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Scene3D
import QtLocation
import QtPositioning
import Qt3D.Core
import Qt3D.Render
import Qt3D.Input
import Qt3D.Extras

ApplicationWindow {
    id: mainWindow

    required property OsmImporterInterface osmImporterInterface
    required property RoadModel roadModel
    required property SceneInterface sceneInterface

    height: 480

    // title of the application
    title: qsTr('Hello World')
    visible: true
    width: 640

    // menu containing two menu items
    menuBar: MenuBar {
        Menu {
            title: qsTr('File')

            MenuItem {
                text: qsTr('&Open')

                onTriggered: console.log('Open action triggered')
            }
            MenuItem {
                text: qsTr('Import from OSM')

                onTriggered: importFromOsmDialog.open()
            }
            MenuItem {
                text: qsTr('Exit')

                onTriggered: Qt.quit()
            }
        }
    }

    OsmImportDialog {
        id: importFromOsmDialog

        osmImporter: mainWindow.osmImporterInterface
    }
    Scene3D {
        anchors.fill: parent
        aspects: ['input', 'logic']
        cameraAspectRatioMode: Scene3D.AutomaticAspectRatio
        focus: true

        Component.onCompleted: {
            mainWindow.sceneInterface.loadScene(scene);
        }

        Entity {
            id: scene

            components: [
                RenderSettings {
                    activeFrameGraph: ForwardRenderer {
                        camera: camera
                        clearColor: Qt.rgba(0, 0.5, 1, 1)
                        showDebugOverlay: true
                    }
                },
                InputSettings {
                }
            ]

            Camera {
                id: camera

                farPlane: 10000.0
                fieldOfView: 45
                nearPlane: 10.0
                position: Qt.vector3d(0.0, 0.0, 200.0)
                projectionType: CameraLens.PerspectiveProjection
                upVector: Qt.vector3d(0.0, 1.0, 0.0)
                viewCenter: Qt.vector3d(0.0, 0.0, 0.0)
            }
            CittyCameraController {
                camera: camera
                linearSpeed: 100
            }
            NodeInstantiator {
                id: roads

                model: mainWindow.roadModel

                delegate: Entity {
                    id: roadDelegate

                    required property vector3d center
                    required property int lanes
                    required property real length
                    required property real rotation

                    components: [sphereMesh, sphereMaterial, sphereTransform]

                    PlaneMesh {
                        id: sphereMesh

                        height: length
                        width: lanes * 3
                        meshResolution: "2x2";
                    }
                    Transform {
                        id: sphereTransform

                        rotationY: roadDelegate.rotation
                        translation: center
                    }
                }
            }
            PhongMaterial {
                id: sphereMaterial

            }
            Entity {
                id: cubeEntity

                components: [cubeTransform, cubeMesh, cubeMaterial]

                Transform {
                    id: cubeTransform

                    translation: Qt.vector3d(0, 0, 0)
                }
                // Cube Mesh
                CuboidMesh {
                    id: cubeMesh

                    xExtent: 1
                    yExtent: 1
                    zExtent: 1
                }
                // Cube Material
                PhongMaterial {
                    id: cubeMaterial

                }
            }
        }
    }
}
