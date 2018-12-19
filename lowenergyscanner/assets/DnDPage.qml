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
import QtQml.Models 2.2
import QtQuick.Layouts 1.3
Rectangle {
    id: root

    width: 320
    height: 480
    property var brickList : []
    Column {
        id: brickSource

        anchors.left: parent.left; anchors.top: parent.top; anchors.bottom: parent.bottom
        anchors.margins: 3
        spacing: 1
        z: dragTarget.z + 1

        Repeater {
            model: 9
            delegate: DragSource { text: "Dummy Brick"; colorKey: "lightblue" }
        }
    }

    ShadowRect {
        id: sr
        dropTarget: dragTarget
    }

    DropArea {
        id: dragTarget
        anchors.top: parent.top
        anchors.left: brickSource.right
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 3
        width: 32; height: 32;

        Rectangle {
            id: dropRectangle

            anchors.fill: parent
            color: "grey"
            border.color: "lightsteelblue"
        }

        function sortVisibleChildren() {
            brickList = []
            for (var i in dragTarget.children) {
                if (dragTarget.children[i].objectName === "Brick")
                    brickList.push(dragTarget.children[i])
            }
            brickList.sort(function(a, b) {
                 return a.y - b.y
            })
            for (var j in brickList) {
                print("j: " + j + " y: " + brickList[j].y)
                let prev = 50
                if (j < 1) brickList[j].y = 50;
                else {
                    brickList[j].y = brickList[j-1].y + 5 + brickList[j-1].height;
                }
            }

        }

    }

}
