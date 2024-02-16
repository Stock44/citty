import main
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtLocation
import QtPositioning

ApplicationWindow {
    id: mainWindow

    required property OsmImporterInterface osmImporter

    height: 480

    //title of the application
    title: qsTr('Hello World')
    visible: true
    width: 640

    //menu containing two menu items
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

        osmImporter: mainWindow.osmImporter
    }

    // Content Area
    StackView {
        id: mainStack

        anchors.fill: parent

        initialItem: Text {
            text: qsTr('Hello world')
        }
    }
}
