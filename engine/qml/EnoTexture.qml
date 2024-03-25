import QtQuick3D

import Eno

Texture {
    property alias data: innerTexture.texture

    textureData: MyCuboidTexture {
        id: innerTexture
    }
}
