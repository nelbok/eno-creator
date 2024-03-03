#pragma once

#include <Qt3DRender/QPaintedTextureImage>

namespace eno {
class Texture;

class CuboidTexture : public Qt3DRender::QPaintedTextureImage {
	Q_OBJECT

public:
	CuboidTexture(Qt3DCore::QNode* parent = nullptr);
	virtual ~CuboidTexture();

	void init(Texture* texture);

protected:
	virtual void paint(QPainter* painter) override;

private:
	Texture* _texture{ nullptr };
};
} // namespace eno
