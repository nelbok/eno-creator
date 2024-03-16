import QtQuick
import QtQuick3D
import QtQuick3D.Helpers

import engine
import Eno

import "."
import "qrc:/engine/qml"

Item {
    width: 1200
    height: 800
    visible: true

    Viewer {
        id: view
        anchors.fill: parent
    }

    Component.onCompleted: {
        view.setModel(MyProject);
    }
}
