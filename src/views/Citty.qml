//import related modules
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

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
                text: qsTr('Exit')
                onTriggered: Qt.quit(); }
        }
    }

    // Content Area
    StackView {
        id: mainStack
        initialItem: WelcomeView {}
        anchors.fill: parent
    }


}


