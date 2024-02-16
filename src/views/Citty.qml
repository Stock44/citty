//import related modules
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtLocation
import QtPositioning

//window containing the application
ApplicationWindow {

    visible: true

    //title of the application
    title: qsTr('Hello World')
    width: 640
    height: 480

    //menu containing two menu items
    menuBar: MenuBar {
        Menu {
            title: qsTr('File')
            MenuItem {
                text: qsTr('&Open')
                onTriggered: console.log('Open action triggered'); }
            MenuItem {
                text: qsTr('Import from OSM'); onTriggered: importFromOsmDialog.open(); }
            MenuItem {
                text: qsTr('Exit')
                onTriggered: Qt.quit(); }
        }
    }

    Dialog {
        id: importFromOsmDialog
        title: 'Import from Open Street Map'
        standardButtons: Dialog.Ok | Dialog.Cancel
        anchors.centerIn: parent
        width: 512
        height: 512

        SelectionArea {
            z: 20
            minWidth: 128
            minHeight: 128
        }

        Map {
            id: map
            plugin: mapPlugin
            center: QtPositioning.coordinate(59.91, 10.75)
            anchors.fill: parent
            z: 10

            PinchHandler {
                id: pinch
                target: null
                onActiveChanged: if (active) {
                    map.startCentroid = map.toCoordinate(pinch.centroid.position, false)
                }
                onScaleChanged: (delta) => {
                    map.zoomLevel += Math.log2(delta)
                    map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
                }
                onRotationChanged: (delta) => {
                    map.bearing -= delta
                    map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
                }
                grabPermissions: PointerHandler.TakeOverForbidden
            }
            WheelHandler {
                id: wheel
                // workaround for QTBUG-87646 / QTBUG-112394 / QTBUG-112432:
                // Magic Mouse pretends to be a trackpad but doesn't work with PinchHandler
                // and we don't yet distinguish mice and trackpads on Wayland either
                acceptedDevices: Qt.platform.pluginName === 'cocoa' || Qt.platform.pluginName === 'wayland' ? PointerDevice.Mouse | PointerDevice.TouchPad : PointerDevice.Mouse
                rotationScale: 1 / 120
                property: 'zoomLevel'
            }
            DragHandler {
                id: drag
                target: null
                onTranslationChanged: (delta) => map.pan(-delta.x, -delta.y)
            }
            Shortcut {
                enabled: map.zoomLevel < map.maximumZoomLevel
                sequence: StandardKey.ZoomIn
                onActivated: map.zoomLevel = Math.round(map.zoomLevel + 1)
            }
            Shortcut {
                enabled: map.zoomLevel > map.minimumZoomLevel
                sequence: StandardKey.ZoomOut
                onActivated: map.zoomLevel = Math.round(map.zoomLevel - 1)
            }
            DropArea {
                anchors.fill: parent
                onDropped: {
                    var coord = map.toCoordinate(Qt.point(drop.x, drop.y));

                    output.text = 'latitude:' + coord.latitude + ', longitude:' + coord.longitude;
                    anim.running = true;
                }
            }
        }
    }

    Plugin {
        id: mapPlugin
        name: 'osm'
        PluginParameter {
            name: 'osm.useragent'; value: 'citty'
        }
    }


    // Content Area
    StackView {
        id: mainStack
        initialItem: Text {
            text: qsTr('Hello world'); }
        anchors.fill: parent
    }


}


