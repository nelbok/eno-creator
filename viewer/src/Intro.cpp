#include "Intro.hpp"

#include <eno/data/Material.hpp>
#include <eno/data/Object.hpp>
#include <eno/data/Scene.hpp>


namespace eno {
void Intro::initIntro() {
	init();
	reset();

	auto* blue = createMaterial("blue", "#0000FF");
	auto* white = createMaterial("white", "#FFFFFF");
	auto* red = createMaterial("red", "#FF0000");

	// E letter
	createObject({ -8, 0, 0 }, blue);
	createObject({ -8, 1, 0 }, blue);
	createObject({ -8, 2, 0 }, blue);
	createObject({ -8, 3, 0 }, blue);
	createObject({ -8, 4, 0 }, blue);
	createObject({ -7, 0, 0 }, blue);
	createObject({ -7, 2, 0 }, blue);
	createObject({ -7, 4, 0 }, blue);
	createObject({ -6, 0, 0 }, blue);
	createObject({ -6, 2, 0 }, blue);
	createObject({ -6, 4, 0 }, blue);
	createObject({ -5, 0, 0 }, blue);
	createObject({ -5, 2, 0 }, blue);
	createObject({ -5, 4, 0 }, blue);
	createObject({ -4, 0, 0 }, blue);
	createObject({ -4, 4, 0 }, blue);

	// N letter
	createObject({ -2, 0, 0 }, white);
	createObject({ -2, 1, 0 }, white);
	createObject({ -2, 2, 0 }, white);
	createObject({ -2, 3, 0 }, white);
	createObject({ -2, 4, 0 }, white);
	createObject({ -1, 3, 0 }, white);
	createObject({ 0, 2, 0 }, white);
	createObject({ 1, 1, 0 }, white);
	createObject({ 2, 0, 0 }, white);
	createObject({ 2, 1, 0 }, white);
	createObject({ 2, 2, 0 }, white);
	createObject({ 2, 3, 0 }, white);
	createObject({ 2, 4, 0 }, white);

	// O letter
	createObject({ 4, 1, 0 }, red);
	createObject({ 4, 2, 0 }, red);
	createObject({ 4, 3, 0 }, red);
	createObject({ 5, 0, 0 }, red);
	createObject({ 5, 4, 0 }, red);
	createObject({ 6, 0, 0 }, red);
	createObject({ 6, 4, 0 }, red);
	createObject({ 7, 0, 0 }, red);
	createObject({ 7, 4, 0 }, red);
	createObject({ 8, 1, 0 }, red);
	createObject({ 8, 2, 0 }, red);
	createObject({ 8, 3, 0 }, red);
}

Material* Intro::createMaterial(const QString& name, const QColor& diffuse) {
	auto* m = new Material(this);
	m->setName(name);
	m->setDiffuse(diffuse);
	add({ m });
	return m;
}

void Intro::createObject(const QVector3D& position, Material* material) {
	auto* o = new Object(this);
	o->setPosition(position);
	o->setMaterial(material);
	scene()->add({ o });
}
} // namespace eno
