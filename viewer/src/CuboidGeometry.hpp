#pragma once

#include <QtQuick3D/QQuick3DGeometry>

// For Q_PROPERTY
#include <eno/data/Project.hpp>

namespace eno {
class CuboidGeometry : public QQuick3DGeometry {
	Q_OBJECT
	Q_PROPERTY(Project* project MEMBER _project WRITE setProject)

public:
	CuboidGeometry(QQuick3DObject* parent = nullptr);
	virtual ~CuboidGeometry() = default;

	void setProject(Project* project);

private:
	void updateData();

	Project* _project{ nullptr };
};
} // namespace eno
