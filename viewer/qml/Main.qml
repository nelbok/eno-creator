import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick3D
import QtQuick3D.Helpers

import Eno
import "."

Window {
    width: 1200
    height: 800
    visible: true

    Viewer {
        id: view
        enabled: !loader.running && !about.visible
        anchors.fill: parent
    }

    Button {
        id: loaderButton
        enabled: !loader.running && !about.visible
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Load a project file..."
        onClicked: loader.open()
    }

    Button {
        id: aboutButton
        enabled: !loader.running && !about.visible
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: loaderButton.right
        anchors.leftMargin: 10
        text: "About"
        onClicked: about.visible = true
    }

    Loader {
        id: loader
        anchors.fill: parent
        onModelChanged: { view.setModel(model); }
    }

    About {
        id: about
        anchors.fill: parent
    }
}
