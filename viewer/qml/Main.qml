import QtQuick
import QtQuick3D
import QtQuick3D.Helpers

import Eno

Window {
    width: 1200
    height: 800
    visible: true

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
            model: MyProject.scene.objects
            delegate: Model {
                source: "#Cube"
                position: Qt.vector3d(model.modelData.position.x * 50, model.modelData.position.y * 50, model.modelData.position.z * 50)
                scale: Qt.vector3d(0.5, 0.5, 0.5)

                materials: [ DefaultMaterial {
                        diffuseColor: model.modelData.material.diffuse
                    }
                ]
            }
        }
    }
}
