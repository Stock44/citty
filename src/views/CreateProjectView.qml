import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    Button {
        text: qsTr('Back')
        onClicked: mainStack.pop()
    }
    TextField {
        placeholderText: qsTr('Name of the project')
    }
    TextField {
        placeholderText: qsTr('Where to save the project')
    }
    TabBar {
        id: bar
        TabButton {
            text: qsTr('From scratch')
        }
        TabButton {
            text: qsTr('From OpenStreetMap data')
        }
    }
    SwipeView {
        Layout.fillWidth: true
        currentIndex: bar.currentIndex
        Item {
            Text {
                text: 'asdfsadf'
            }
        }
        Item {
            Text {
                text: 'ioioioioi'
            }
        }
    }
}
