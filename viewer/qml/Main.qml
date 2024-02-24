import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick3D
import QtQuick3D.Helpers

import Eno

Window {
    width: 1200
    height: 800
    visible: true

    MyLoader {
        id: myLoader

        function openProject(filePath) {
            progressBar.visible = true
            scene3D.model = MyIntro.scene.objects;
            init(MyProject, MyLoader.Type.Load, filePath);
            start();
        }

        onFinished: {
            if (result() === MyLoader.Result.Success)
                scene3D.model = MyProject.scene.objects;
            progressBar.visible = false
        }
    }

    // https://doc.qt.io/qt-6/qtquick3dphysics-units.html
    // Qt Quick 3D Unit: cm
    View3D {
        id: view
        enabled: !progressBar.visible
        anchors.fill: parent

        environment: SceneEnvironment {
            clearColor: "skyblue"
            backgroundMode: SceneEnvironment.Color
        }

        // A light like the sun
        DirectionalLight {
            eulerRotation.x: -30
            eulerRotation.y: -70
            castsShadow: true
        }

        // Camera with its controller to make it easy to move around the scene
        PerspectiveCamera {
            id: camera
            position: Qt.vector3d(0, 800, 1000)
            eulerRotation.x: -30
        }
        WasdController {
            controlledObject: camera
        }

        Repeater3D {
            id: scene3D
            model: MyIntro.scene.objects
            delegate: Model {
                source: "#Cube"
                // Eno is in meter, we need to convert into cm
                position: Qt.vector3d(model.modelData.position.x * 100, model.modelData.position.y * 100, model.modelData.position.z * 100)

                materials: [ DefaultMaterial {
                        diffuseColor: model.modelData.material.diffuse
                    }
                ]
            }
        }
    }

    Button {
        id: button
        enabled: !progressBar.visible
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Load a project file..."
        onClicked: fileDialog.open();
    }

    Rectangle {
        id: progressBar
        visible: false
        anchors.fill: parent
        color: "#AAAAAAAA"

        BusyIndicator {
            anchors.centerIn: parent
            running: true
        }
    }

    FileDialog {
        id: fileDialog
        nameFilters: ["Eno Project (*.eno)"]
        onAccepted: {
            myLoader.openProject(selectedFile)
        }
    }
}
