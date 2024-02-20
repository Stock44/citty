import citty
import QtQuick
import QtQuick.Controls
import QtLocation
import QtPositioning

Dialog {
    id: dialog

    required property OsmImporterInterface osmImporter

    anchors.centerIn: parent
    height: 512
    standardButtons: Dialog.Ok | Dialog.Cancel
    title: 'Import from Open Street Map'
    width: 512

    onAccepted: {
        const left = selectionArea.x;
        const right = left + selectionArea.width;
        const top = selectionArea.y;
        const bottom = top + selectionArea.height;
        const c1 = map.toCoordinate(Qt.point(left, top));
        const c2 = map.toCoordinate(Qt.point(right, bottom));
        osmImporter.importOsmRegion(c1, c2);
    }

    Plugin {
        id: mapPlugin

        name: 'osm'

        PluginParameter {
            name: 'osm.useragent'
            value: 'citty'
        }
    }
    SelectionArea {
        id: selectionArea

        minHeight: 128
        minWidth: 128
        z: 20
    }
    Map {
        id: map

        anchors.fill: parent
        center: QtPositioning.coordinate(59.91, 10.75)
        plugin: mapPlugin
        z: 10

        PinchHandler {
            id: pinch

            grabPermissions: PointerHandler.TakeOverForbidden
            target: null

            onActiveChanged: if (active) {
                map.startCentroid = map.toCoordinate(pinch.centroid.position, false);
            }
            onScaleChanged: delta => {
                map.zoomLevel += Math.log2(delta);
                map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position);
            }
        }
        WheelHandler {
            id: wheel

            // workaround for QTBUG-87646 / QTBUG-112394 / QTBUG-112432:
            // Magic Mouse pretends to be a trackpad but doesn't work with PinchHandler
            // and we don't yet distinguish mice and trackpads on Wayland either
            acceptedDevices: Qt.platform.pluginName === 'cocoa' || Qt.platform.pluginName === 'wayland' ? PointerDevice.Mouse | PointerDevice.TouchPad : PointerDevice.Mouse
            property: 'zoomLevel'
            rotationScale: 1 / 120
        }
        DragHandler {
            id: drag

            target: null

            onTranslationChanged: delta => map.pan(-delta.x, -delta.y)
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
    }
}
