import QtQuick
import QtQuick3D
import QtQuick3D.Helpers

Window {
    width: 1200
    height: 800
    visible: true
    title: qsTr("Hello World")

    ListModel {
        id: enoData
        ListElement { x: 0; y: 0; z: 0; color: "red" }
        ListElement { x: 2; y: 0; z: 0; color: "blue" }
        ListElement { x: 0; y: 0; z: 2; color: "pink" }
        ListElement { x: 2; y: 0; z: 2; color: "yellow" }
        ListElement { x: 1; y: 1; z: 1; color: "black" }
    }

    View3D {
        id: view
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
            position: Qt.vector3d(50, 200, 300)
            eulerRotation.x: -30
        }
        WasdController {
            controlledObject: camera
        }

        Repeater3D {
            model: enoData
            delegate: Model {
                source: "#Cube"
                position: Qt.vector3d(enoData.get(index).x * 50, enoData.get(index).y * 50, enoData.get(index).z * 50)
                scale: Qt.vector3d(0.5, 0.5, 0.5)

                materials: [ DefaultMaterial {
                        diffuseColor: enoData.get(index).color
                    }
                ]
            }
        }
    }
}
