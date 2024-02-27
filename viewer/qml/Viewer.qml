import QtQuick3D
import QtQuick3D.Helpers

import Eno

// https://doc.qt.io/qt-6/qtquick3dphysics-units.html
// Qt Quick 3D Unit: cm
View3D {
    id: root
    property var model: null

    environment: SceneEnvironment {
        clearColor: "skyblue"
        backgroundMode: SceneEnvironment.Color
    }

    // A light like the sun
    DirectionalLight {
        eulerRotation.x: -30
        eulerRotation.y: -70
        castsShadow: true
        shadowMapQuality: Light.ShadowMapQualityVeryHigh
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
        model: root.model.materials
        delegate: Model {
            geometry: MyCuboidGeometry {
                scene: root.model.scene
                material: model.modelData
            }
            // Eno is in meter, we need to convert into cm
            scale: Qt.vector3d(100,100,100)

            materials: [ DefaultMaterial {
                    diffuseColor: model.modelData.diffuse
                }
            ]
        }
    }
}

