import QtQuick 2.0

Rectangle {

    property Item dropTarget
//    property Rectangle dropRectangle
    id: shadowRect
    // used as the valid drop position indicator
    visible: false
    width: 100
    height: 100
    color: "red"
//    z : 1
    parent: dropTarget
    function steppize(val) {
        return Math.floor(val/20)*20
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
                x: steppize(dropTarget.drag.x)//dropTarget.drag.source.x - width//steppize(mapToItem(dropRectangle, dropTarget.drag.source.x, 0).x)
                y: steppize(dropTarget.drag.y)//mapY(dropTarget.drag.source)//steppize(mapFromItem(dropRectangle, 0, dropTarget.drag.source.y).y)
            }
        }
    ]
}
