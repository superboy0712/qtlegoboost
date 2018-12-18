/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.11

//! [0]
Item {
    id: root
    property string colorKey
    width: 64; height: 32
    Rectangle {
        id: sourceAnchor
        anchors.fill: parent
        color: "lightgreen"
        opacity: 0.4
    }

    MouseArea {
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
                console.log(sr.x , sr.y, "vs: ", mouseArea.x, mouseArea.y)
                parent = tile.Drag.target !== null ? tile.Drag.target : root
                if (tile.Drag.target === null) print("nulllll")
                mouseArea.x = sr.x
                mouseArea.y = sr.y
                console.log("after: ", sr.x , sr.y, "vs: ", mouseArea.x, mouseArea.y)
            }
        }

        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onDoubleClicked: {
            console.log("double clicked to delete:", index)
            if(mouse.button & Qt.RightButton) {
                if (root.children.length > 3) {
                    tile.destroy()
                    mouseArea.destroy()
                } else {
                    parent = root
                    x = sourceAnchor.x
                    y = sourceAnchor.y
                }
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
            Drag.keys: [ colorKey ]
            Drag.active: mouseArea.drag.active
            Drag.hotSpot.x: 0
            Drag.hotSpot.y: 0
//! [0]
            Text {
                anchors.fill: parent
                color: "white"
                font.pixelSize: 24
                text: modelData + 1
                horizontalAlignment:Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
//! [1]
            states: State {
                when: mouseArea.drag.active
                AnchorChanges { target: tile; anchors.verticalCenter: undefined; anchors.horizontalCenter: undefined }
            }

        }
    }
}
//! [1]

