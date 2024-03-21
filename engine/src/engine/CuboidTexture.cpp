#include <eno/engine/CuboidTexture.hpp>

namespace eno {

CuboidTexture::CuboidTexture(QQuick3DObject* parent)
	: QQuick3DTextureData(parent) {
	setFormat(QQuick3DTextureData::RGBA8);
	setHasTransparency(true);
	updateData();
}

CuboidTexture::~CuboidTexture() {}

void CuboidTexture::setTexture(Texture* texture) {
	_texture = texture;
	updateData();
}

void CuboidTexture::updateData() {
	if (_texture) {
		setSize(_texture->size());
		setTextureData(_texture->data());
	} else {
		defaultTexture();
	}
	update();
}

void CuboidTexture::defaultTexture() {
#if 0
	QByteArray tex;
	for (int i = 0; i < 512; ++i) {
		QByteArray line;
		line.resize(4 * 512); // RGBA8
		for (int j = 0; j < 512; ++j) {
			line.data()[j * 4 + 0] = char(255);
			line.data()[j * 4 + 1] = char(255);
			line.data()[j * 4 + 2] = char(255);
			line.data()[j * 4 + 3] = char(127);
		}
		tex += line;
	}
	setTextureData(tex);
	setSize({ 512, 512 });
#else
	QByteArray dot;
	dot.resize(4); // RGBA8
	dot.data()[0] = char(255);
	dot.data()[1] = char(255);
	dot.data()[2] = char(255);
	dot.data()[3] = char(255);
	setTextureData(dot);
	setSize({ 1, 1 });
#endif
}
} // namespace eno
