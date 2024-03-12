#include "CuboidTexture.hpp"

#include <QPainter>

#include <eno/data/Texture.hpp>

namespace eno {

CuboidTexture::CuboidTexture(QNode* parent)
	: QPaintedTextureImage(parent) {}

CuboidTexture::~CuboidTexture() {}


void CuboidTexture::init(Texture* texture) {
	assert(texture);
	_texture = texture;
	setSize({ 64, 64 });
}

void CuboidTexture::paint(QPainter* painter) {
	auto image = _texture->image();
	image.mirror();
	painter->drawImage(QRectF({ 0, 0 }, size()), image, image.rect());
}
} // namespace eno
