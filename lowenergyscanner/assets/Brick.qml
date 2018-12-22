import QtQuick 2.10
import QtQuick.Controls 2.4

MouseArea {
    property bool busy: brickItem.busy
    property var payLoad: brickItem.payLoad
    property alias timer: timer
    readonly property int duration: 1500
    function destroyOrReset() {
        if (brickItem.children.length > 3) {
            for (var i in mouseArea.children) {
                mouseArea.children[i].destroy()
            }
            mouseArea.destroy()
        } else {
            mouseArea.parent = brickItem
            mouseArea.x = sourceAnchorRect.x
            mouseArea.y = sourceAnchorRect.y
        }
    }
    onClicked: {
        if(mouse.button & Qt.RightButton) {
            console.log("right clicked to delete:", objectName)
            destroyOrReset()
        }
    }
    Timer {
        id: timer
        interval: 0
        repeat: true
        onTriggered: {
            if (!busy) {
                busy = true
                interval= duration
                tile.color = "red"
                tile.opacity = 0.3
                payLoad()
                repeat = false
            }
            else {
                tile.color = colorKey
                tile.opacity = 1
                busy = false
                repeat = true
                interval = 0
            }
        }
    }

    id: mouseArea
    objectName: "Brick"
    Rectangle {
        id: maRect
        anchors.fill: parent
        color: "lightsteelblue"
        opacity: 0.3
    }
    width: brickItem.width; height: brickItem.height
    z: (drag.active) ? 1 : 0 // raise above siblings while being dragged
    drag.target: tile
    onReleased: {
        if(mouse.button & Qt.LeftButton) {
            if (tile.Drag.target === null) return;
            parent = tile.Drag.target
            mouseArea.x = shadowRect.x
            mouseArea.y = shadowRect.y
        }
        dropTarget.sortVisibleChildren()
    }

    acceptedButtons: Qt.LeftButton | Qt.RightButton
    onDoubleClicked: {
        if(mouse.button & Qt.LeftButton) {
            console.log("double left clicked to Run Code!:", objectName)
            timer.interval = 100 // 0 too soon to run
            timer.start();
        }
    }

    Rectangle {
        id: tile

        width: brickItem.width; height: brickItem.height
        anchors.verticalCenter: mouseArea.verticalCenter
        anchors.horizontalCenter: mouseArea.horizontalCenter

        color: colorKey
        border.color: "grey"
        Drag.active: mouseArea.drag.active
        Drag.hotSpot.x: 0
        Drag.hotSpot.y: 0
//! [0]
        Text {
            anchors.fill: parent
            color: "white"
            font.pixelSize: 24
            text: brickItem.text
            horizontalAlignment:Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

//        TextField {
//            placeholderText: timer.interval
//            anchors.top: tile.top
//            anchors.left: tile.right
//            anchors.bottom: tile.bottom
//            anchors.margins: 3
//            width: tile.width * 0.19
//            validator: IntValidator {bottom: 10; top: 10000;}
//        }
//! [1]
        states: State {
            when: mouseArea.drag.active
            AnchorChanges { target: tile; anchors.verticalCenter: undefined; anchors.horizontalCenter: undefined }
        }
    }
}
