#include "CuboidTexture.hpp"

namespace eno {

CuboidTexture::CuboidTexture(QQuick3DObject* parent)
	: QQuick3DTextureData(parent) {
	clear();
}

CuboidTexture::~CuboidTexture() {
	clear();
}

void CuboidTexture::setTexture(Texture* texture) {
	_texture = texture;
	updateData();
}

void CuboidTexture::clear() {
	setSize({ 1, 1 });

	QByteArray data;
	data.resize(4); // RGBA8
	data.data()[0] = char(255);
	data.data()[1] = char(255);
	data.data()[2] = char(255);
	data.data()[3] = char(255);
	setTextureData(data);
}

void CuboidTexture::updateData() {
	clear();
	if (!_texture) {
		return;
	}

	setSize(_texture->size());
	setTextureData(_texture->data());
}
} // namespace eno
