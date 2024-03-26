#pragma once

#include <QtQuick3D/QQuick3DGeometry>

// For Q_PROPERTY
#include <eno/data/Project.hpp>

namespace eno {
class CuboidGeometry : public QQuick3DGeometry {
	Q_OBJECT
	Q_PROPERTY(Project* project MEMBER _project WRITE setProject)
	Q_PROPERTY(bool optimized MEMBER _optimized WRITE setOptimized)

public:
	CuboidGeometry(QQuick3DObject* parent = nullptr);
	virtual ~CuboidGeometry();

	void setProject(Project* project);
	void setOptimized(bool optimized);

private:
	void updateData();

	Project* _project{ nullptr };
	bool _optimized{ false };
};
} // namespace eno
