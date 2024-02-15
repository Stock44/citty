import QtQuick
import QtQuick.Controls

Rectangle {
    id: selectionArea
    border {
        width: 2
        color: 'steelblue'
    }
    color: '#354682B4'

    property int rulersSize: 18

    MouseArea {     // drag mouse area
        anchors.fill: parent
        drag {
            target: parent
            minimumX: 0
            minimumY: 0
            maximumX: parent.parent.width - parent.width
            maximumY: parent.parent.height - parent.height
            smoothed: true
        }

        onDoubleClicked: {
            parent.destroy()        // destroy component
        }
    }

    Rectangle {
        width: rulersSize
        height: rulersSize
        radius: rulersSize
        color: 'steelblue'
        anchors.horizontalCenter: parent.left
        anchors.verticalCenter: parent.verticalCenter

        MouseArea {
            anchors.fill: parent
            drag {
                target: parent; axis: Drag.XAxis
            }
            onMouseXChanged: {
                if (drag.active) {
                    selectionArea.width = selectionArea.width - mouseX
                    selectionArea.x = selectionArea.x + mouseX
                    if (selectionArea.width < 30)
                        selectionArea.width = 30
                }
            }
        }
    }

    Rectangle {
        width: rulersSize
        height: rulersSize
        radius: rulersSize
        color: 'steelblue'
        anchors.horizontalCenter: parent.right
        anchors.verticalCenter: parent.verticalCenter

        MouseArea {
            anchors.fill: parent
            drag {
                target: parent; axis: Drag.XAxis
            }
            onMouseXChanged: {
                if (drag.active) {
                    selectionArea.width = selectionArea.width + mouseX
                    if (selectionArea.width < 50)
                        selectionArea.width = 50
                }
            }
        }
    }

    Rectangle {
        width: rulersSize
        height: rulersSize
        radius: rulersSize
        x: parent.x / 2
        y: 0
        color: 'steelblue'
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.top

        MouseArea {
            anchors.fill: parent
            drag {
                target: parent; axis: Drag.YAxis
            }
            onMouseYChanged: {
                if (drag.active) {
                    selectionArea.height = selectionArea.height - mouseY
                    selectionArea.y = selectionArea.y + mouseY
                    if (selectionArea.height < 50)
                        selectionArea.height = 50
                }
            }
        }
    }


    Rectangle {
        width: rulersSize
        height: rulersSize
        radius: rulersSize
        x: parent.x / 2
        y: parent.y
        color: 'steelblue'
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.bottom

        MouseArea {
            anchors.fill: parent
            drag {
                target: parent; axis: Drag.YAxis
            }
            onMouseYChanged: {
                if (drag.active) {
                    selectionArea.height = selectionArea.height + mouseY
                    if (selectionArea.height < 50)
                        selectionArea.height = 50
                }
            }
        }
    }
}
