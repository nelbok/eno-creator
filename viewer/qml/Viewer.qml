import QtQuick3D
import QtQuick3D.Helpers

// https://doc.qt.io/qt-6/qtquick3dphysics-units.html
// Qt Quick 3D Unit: cm
View3D {
    id: view
    property alias model: scene3D.model

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

