import QtQuick3D

import Eno

DefaultMaterial {
    property alias diffuseTexture: cuboidTexture.texture

    diffuseMap: Texture {
        textureData: MyCuboidTexture {
            id: cuboidTexture
        }
    }
}
