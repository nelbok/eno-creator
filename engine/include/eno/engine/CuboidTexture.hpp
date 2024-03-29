#pragma once

#include <QtQuick3D/QQuick3DTextureData>

// For Q_PROPERTY
#include <eno/data/Texture.hpp>

namespace eno {
class CuboidTexture : public QQuick3DTextureData {
	Q_OBJECT
	Q_PROPERTY(Texture* texture MEMBER _texture WRITE setTexture)

public:
	CuboidTexture(QQuick3DObject* parent = nullptr);
	virtual ~CuboidTexture();

	void setTexture(Texture* texture);

private:
	void updateData();
	void defaultTexture();

	Texture* _texture{ nullptr };
};
} // namespace eno
