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
    property int minWidth: 0
    property int minHeight: 0

    width: minWidth
    height: minHeight

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
        id: leftHandle
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
                    const updatedWidth = selectionArea.width - mouseX;
                    const updatedX = selectionArea.x + mouseX
                    if (updatedX >= 0 && updatedWidth >= minWidth) {
                        selectionArea.width = updatedWidth
                        selectionArea.x = updatedX
                    }


                    if (selectionArea.width < minWidth) selectionArea.width = minWidth
                }
            }
        }
    }

    Rectangle {
        id: rightHandle
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
                    const updatedWidth = selectionArea.width + mouseX;

                    if (selectionArea.x + updatedWidth <= selectionArea.parent.width) {
                        selectionArea.width = updatedWidth;
                    }

                    if (selectionArea.width < minWidth) selectionArea.width = minWidth;
                }
            }
        }
    }

    Rectangle {
        id: topHandle
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
                    const updatedHeight = selectionArea.height - mouseY;
                    const updatedY = selectionArea.y + mouseY

                    if (updatedY >= 0 & updatedHeight >= minHeight) {
                        selectionArea.height = updatedHeight;
                        selectionArea.y = updatedY;
                    }

                    if (selectionArea.height < minHeight) selectionArea.height = minHeight
                }
            }
        }
    }


    Rectangle {
        id: bottomHandle
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
                    const updatedHeight = selectionArea.height + mouseY;

                    if (selectionArea.y + updatedHeight <= selectionArea.parent.height) {
                        selectionArea.height = updatedHeight;
                    }
                    if (selectionArea.height < minHeight) selectionArea.height = minHeight
                }
            }
        }
    }
}
