import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    anchors.top: parent.top
    spacing: 0

    RowLayout {
        Layout.fillWidth: true
        Layout.fillHeight: false
        Layout.margins: 8
        TextField {
            placeholderText: qsTr('Filter projects')
            Layout.minimumWidth: 256
            Layout.maximumWidth: 512
        }
        Item {
            Layout.fillWidth: true
        }
        Button {
            text: qsTr('New project')
            onClicked: mainStack.push('CreateProjectView.qml')
        }
        Button {
            text: qsTr('Load a project')
        }
    }
    ColumnLayout {
        Layout.margins: 8
        Layout.fillHeight: true
        Layout.fillWidth: true
    }
}
