#pragma once

#include <eno/data/Project.hpp>

class QString;
class QColor;

namespace eno {
class Material;
class Intro : public Project {
public:
	void initIntro();

private:
	Material* createMaterial(const QString& name, const QColor& diffuse);
	void createObject(const QVector3D& position, Material* material);
};
} // namespace eno
