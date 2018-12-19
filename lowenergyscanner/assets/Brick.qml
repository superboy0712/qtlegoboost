import QtQuick 2.10

MouseArea {
    function destroyOrReset() {
        print("destroyOrReset()")
        if (root.children.length > 3) {
            tile.destroy()
            mouseArea.destroy()
        } else {
            mouseArea.parent = root
            mouseArea.x = sourceAnchor.x
            mouseArea.y = sourceAnchor.y
        }
    }
    id: mouseArea
    Rectangle {
        id: maRect
        anchors.fill: parent
        color: "lightsteelblue"
        opacity: 0.3
    }
    width: root.width; height: root.height
    drag.target: tile
    onReleased: {
        if(mouse.button & Qt.LeftButton) {
            if (tile.Drag.target === null) return;
            parent = tile.Drag.target !== null ? tile.Drag.target : root
            mouseArea.x = sr.x
            mouseArea.y = sr.y
        }
    }

    acceptedButtons: Qt.LeftButton | Qt.RightButton
    onDoubleClicked: {
        if(mouse.button & Qt.RightButton) {
            console.log("double clicked to delete:", index, objectName)
            destroyOrReset()
        }

        if(mouse.button & Qt.LeftButton) {
            console.log("double clicked to Run Code!:", index, objectName)
            root.payLoad()
        }
    }


    Binding on anchors.verticalCenter {
        when: parent !== root
        value: undefined
    }

    Binding on anchors.verticalCenter {
        when: parent === root
        value: root.verticalCenter
    }

    Binding on anchors.horizontalCenter {
        when: parent !== root
        value: undefined
    }

    Binding on anchors.horizontalCenter {
        when: parent === root
        value: root.horizontalCenter
    }

    Rectangle {
        id: tile

        width: root.width; height: root.height
        anchors.verticalCenter: mouseArea.verticalCenter
        anchors.horizontalCenter: mouseArea.horizontalCenter

        color: colorKey
        border.color: "grey"
//        Drag.keys: [ colorKey ]
        Drag.active: mouseArea.drag.active
        Drag.hotSpot.x: 0
        Drag.hotSpot.y: 0
//! [0]
        Text {
            anchors.fill: parent
            color: "white"
            font.pixelSize: 24
            text: root.text + " " + modelData
            horizontalAlignment:Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
//! [1]
        states: State {
            when: mouseArea.drag.active
            AnchorChanges { target: tile; anchors.verticalCenter: undefined; anchors.horizontalCenter: undefined }
        }

        Binding on z {
            when: Drag.active
            value: parent.z + 2
        }

        Binding on z {
            when: !Drag.active
            value: parent.z
        }

    }
}
