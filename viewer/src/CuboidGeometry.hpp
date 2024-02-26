#pragma once

#include <QtQuick3D/QQuick3DGeometry>

// For Q_PROPERTY
#include <eno/data/Material.hpp>
#include <eno/data/Scene.hpp>

namespace eno {
class CuboidGeometry : public QQuick3DGeometry {
	Q_OBJECT
	Q_PROPERTY(Material* material MEMBER _material WRITE setMaterial)
	Q_PROPERTY(Scene* scene MEMBER _scene WRITE setScene)

public:
	CuboidGeometry(QQuick3DObject* parent = nullptr);
	virtual ~CuboidGeometry() = default;

	void setMaterial(Material* material);
	void setScene(Scene* scene);

private:
	void updateData();

	Material* _material{ nullptr };
	Scene* _scene{ nullptr };
};
} // namespace eno
