import QtQuick
import QtQuick3D
import QtQuick3D.Helpers

import Eno

// https://doc.qt.io/qt-6/qtquick3dphysics-units.html
// Qt Quick 3D Unit: cm
View3D {
    id: root

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
    PinchHandler {
        target: null
        onScaleChanged: (delta) => {
                            var p = camera.position;
                            var f = camera.forward;
                            var factor = 0;
                            if (delta > 1) {
                                factor = 10;
                            } else {
                                factor = -10;
                            }
                            p.x = p.x + f.x * factor;
                            p.y = p.y + f.y * factor;
                            p.z = p.z + f.z * factor;
                        }
    }

    function setModel(model) {
        // Clear old materials
        for (var m in myModel.matList) {
            myModel.matList[m].destroy();
        }
        myModel.matList = [];

        // Clear old textures
        for (var t in myModel.texList) {
            myModel.texList[t].destroy();
        }
        myModel.texList = [];

        // Set new textures
        var compTex = Qt.createComponent("EnoTexture.qml");
        if (compTex.status === Component.Ready) {
            for (var et in model.textures) {
                var objTex = compTex.createObject(myModel);
                objTex.data = model.textures[et];
                myModel.texList.push(objTex);
            }
        }

        // Set new materials
        var compMat = Qt.createComponent("EnoMaterial.qml");
        if (compMat.status === Component.Ready) {
            for (var em in model.materials) {
                var mat = model.materials[em];

                var objMat = compMat.createObject(myModel);
                objMat.baseColor = mat.diffuse;
                objMat.opacity = mat.opacity;
                for(var mt in model.textures) {
                    if (model.textures[mt] === mat.diffuseMap) {
                        objMat.baseColorMap = myModel.texList[mt];
                    }
                    if (model.textures[mt] === mat.opacityMap) {
                        objMat.opacityMap = myModel.texList[mt];
                    }
                }

                myModel.matList.push(objMat);
            }
        }

        // Set new model
        geometry.project = model;
    }

    Model {
        id: myModel
        property list<Texture> texList
        property list<PrincipledMaterial> matList

        materials: matList
        geometry: MyCuboidGeometry {
            id: geometry
        }

        // Eno is in meter, we need to convert into cm
        scale: Qt.vector3d(100,100,100)
    }

    Model {
        id: debug
        visible: false
        position: Qt.vector3d(0, -100, 0)
        scale: Qt.vector3d(10,1,10)
        source: "#Cube"
        materials: DefaultMaterial {
            diffuseColor: "green"
        }
    }
}
