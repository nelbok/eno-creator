import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

import Eno

Rectangle {
    id: root
    visible: false
    color: "#AAAAAAAA"

    Rectangle {
        color: "#FFFFFF"
        anchors.centerIn: parent
        width: 400
        height: 300

        Image {
            id: logo
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 10
            source: MyAbout.logo
            width: 60
            height: 60
        }

        Button {
            id: button
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            text: "OK"
            onClicked: {
                root.visible = false
            }
        }

        Text {
            anchors.left: logo.right
            anchors.leftMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: button.top
            anchors.bottomMargin: 10
            wrapMode: Text.WordWrap
            text: MyAbout.text
        }
    }
}
