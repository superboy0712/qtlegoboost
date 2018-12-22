import QtQuick 2.0

Rectangle {
    property Item dropTarget
    id: shadowRect
    // used as the valid drop position indicator
    visible: false
    width: 100
    height: 100
    parent: dropTarget
    anchors.horizontalCenter: dropTarget.horizontalCenter
    function steppize(val) {
        return Math.floor(val/16)*16
    }

    states: [
        State {
            when: dropTarget.containsDrag
            PropertyChanges {
                target: shadowRect
                visible: true
                color: "orange"
                opacity: 0.4
                width: dropTarget.drag.source.width
                height: dropTarget.drag.source.height
                y: steppize(dropTarget.drag.y)
            }
        }
    ]
}
