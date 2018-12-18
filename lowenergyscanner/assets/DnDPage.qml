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

import QtQuick 2.0

Rectangle {
    id: root

    width: 320
    height: 480

//    color: "black"

    Column {
        id: redSource

        anchors.left: parent.left; anchors.top: parent.top; anchors.bottom: parent.bottom
        anchors.margins: 3
        spacing: 1
        z: dragTarget.z + 1

        Repeater {
            model: 9
            delegate: DragSource { colorKey: "lightblue" }
        }
    }

    DropArea {
        id: dragTarget
        anchors.top: parent.top
        anchors.left: redSource.right
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 3
        property string colorKey: "grey"
        property alias dropProxy: dragTarget
        width: 32; height: 32;
//        keys: [ "color", "width" ]

        Rectangle {
            id: shadowRect
            // used as the valid drop position indicator
            visible: false
            width: 100
            height: 100
            color: "red"
            z : 1
            anchors.centerIn: parent
            states: [
                State {
                    when: dragTarget.containsDrag
                    PropertyChanges {
                        target: shadowRect
                        visible: true
                        color: "orange"
                        width: dragTarget.drag.source.width
                        height: dragTarget.drag.source.height
                    }
                }
            ]
        }

        Rectangle {
            id: dropRectangle

            anchors.fill: parent
            color: "grey"
            border.color: "lightsteelblue"
            states: [
                State {
                    when: contains(Qt.point(drag.source.x, drag.source.y))//drag.x + drag.source.width >= dropRectangle.x && drag.x + drag.source.width <= dropRectangle.x + dropRectangle.width
                    PropertyChanges {
                        target: dropRectangle
                        color: "grey"
                    }
                }
            ]
        }
    }

}
