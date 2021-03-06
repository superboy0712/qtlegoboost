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
import QtQuick.Controls 2.4
Rectangle {
    id: dndPage

    width: 320
    height: 480
    property var brickList : []
    readonly property int dropAreaSpacing : 8
    Column {
        id: brickSource

        anchors.left: parent.left; anchors.top: parent.top; anchors.bottom: parent.bottom
        anchors.margins: 3
        spacing: 1
        z: dropTarget.z + 1
        PrimaryBrick {
            text: "Forward"
            payLoad: function() {motorAB.move(1000, -100, -100); }
        }

        PrimaryBrick {
            text: "Backwards"
            payLoad: function() {motorAB.move(1000, 100, 100); }
        }

        PrimaryBrick {
            text: "Rotate"
            payLoad: function() {motorCD.rotate(720); }
        }

        PrimaryBrick {
            text: "Color"
            colorKey: device.valueColor
            payLoad: function() {print(device.valueColor); colorKey = device.valueColor}
        }

        PrimaryBrick {
            text: "Turn Right"
            payLoad: function() {motorAB.move(1000, 0, -100); }
        }

        PrimaryBrick {
            text: "Turn Left"
            payLoad: function() {motorAB.move(1000, -100, 0); }
        }

        PrimaryBrick {
            text: "Reverse Left"
            payLoad: function() {motorAB.move(1000, 100, 0); }
        }

        PrimaryBrick {
            text: "Reverse Right"
            payLoad: function() {motorAB.move(1000, 0, 100); }
        }

    }

    ShadowRect {
        id: shadowRect
        dropTarget: dropTarget
    }

    Button {
        anchors.margins: 20
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        text: "Run"
        onClicked: dropTarget.runBrickList()
    }

    DropArea {
        id: dropTarget
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
            for (var i in dropTarget.children) {
                if (dropTarget.children[i].objectName === "Brick")
                    brickList.push(dropTarget.children[i])
            }
            brickList.sort(function(a, b) {
                 return a.y - b.y
            })
            for (var j in brickList) {
                if (j < 1) brickList[j].y = dropAreaSpacing * 10;
                else {
                    brickList[j].y = brickList[j-1].y + dropAreaSpacing + brickList[j-1].height;
                }
            }

        }

        function runBrickList() {
            for (var i in brickList) {
                brickList[i].timer.interval =  300 + 1500 * (i);
                brickList[i].timer.start()
            }
        }

    }

}
