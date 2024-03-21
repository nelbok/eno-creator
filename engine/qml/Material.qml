import QtQuick3D

import Eno

PrincipledMaterial {
    property alias baseColorTexture: matBaseColorTexture.texture
//    property alias opacityTexture: matOpacityTexture.texture

    alphaMode: PrincipledMaterial.Mask
    baseColorMap: Texture {
        textureData: MyCuboidTexture {
            id: matBaseColorTexture
        }
    }
    // opacityMap: Texture {
    //     textureData: MyCuboidTexture {
    //         id: matOpacityTexture
    //     }
    // }
}
