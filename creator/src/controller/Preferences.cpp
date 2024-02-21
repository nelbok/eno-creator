#include "Preferences.hpp"

#include <QtCore/QSettings>
#include <QtCore/QStandardPaths>

#include <eno/data/Scene.hpp>

namespace eno {

QString Preferences::toString(Preferences::Zoom zoom) {
	switch (zoom) {
		case Preferences::Zoom::x25:
			return " x25";
		case Preferences::Zoom::x50:
			return " x50";
		case Preferences::Zoom::x100:
			return " x100";
		case Preferences::Zoom::x200:
			return " x200";
	}
	assert(false);
	return {};
}

// Scene
QPoint Preferences::sceneMin() {
	return QSettings().value("scene/min", Scene::defaultSceneMin).toPoint();
}

QPoint Preferences::sceneMax() {
	return QSettings().value("scene/max", Scene::defaultSceneMax).toPoint();
}

void Preferences::setSceneMin(const QPoint& min) {
	QSettings().setValue("scene/min", min);
}

void Preferences::setSceneMax(const QPoint& max) {
	QSettings().setValue("scene/max", max);
}

// Map
int Preferences::mapDepth() {
	return QSettings().value("map/depth", 0.f).toInt();
}

int Preferences::mapPenWidth() {
	return QSettings().value("map/penwidth", 1).toInt();
}

Preferences::Zoom Preferences::mapZoom() {
	return QSettings().value("map/zoom", QVariant::fromValue(Zoom::x100)).value<Preferences::Zoom>();
}

void Preferences::setMapDepth(int depth) {
	QSettings().setValue("map/depth", depth);
}

void Preferences::setMapPenWidth(int penWidth) {
	QSettings().setValue("map/penwidth", penWidth);
}

void Preferences::setMapZoom(Zoom zoom) {
	QSettings().setValue("map/zoom", QVariant::fromValue(zoom));
}

// Material
QString Preferences::materialName() {
	return QSettings().value("material/name", "My material").toString();
}

void Preferences::setMaterialName(const QString& name) {
	QSettings().setValue("material/name", name);
}

QColor Preferences::materialDiffuse() {
	return QSettings().value("material/diffuse", QColor("#ffaa00")).value<QColor>();
}

void Preferences::setMaterialDiffuse(const QColor& diffuse) {
	QSettings().setValue("material/diffuse", diffuse);
}

// Locations
QString Preferences::projectLocation() {
	return QSettings().value("locations/eno", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();
}

QString Preferences::generateLocation() {
	return QSettings().value("locations/wavefront", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();
}

void Preferences::setProjectLocation(const QString& location) {
	QSettings().setValue("locations/eno", location);
}

void Preferences::setGenerateLocation(const QString& location) {
	QSettings().setValue("locations/wavefront", location);
}

// Optimization
bool Preferences::generationOptimized() {
	return QSettings().value("optimization/wavefront", true).toBool();
}

void Preferences::setGenerationOptimized(bool enabled) {
	QSettings().setValue("optimization/wavefront", enabled);
}

// Key binding
Qt::Key Preferences::keyRemove() {
	return QSettings().value("keybinding/remove", Qt::Key_1).value<Qt::Key>();
}

void Preferences::setKeyRemove(Qt::Key key) {
	QSettings().setValue("keybinding/remove", key);
}

Qt::Key Preferences::keyAdd() {
	return QSettings().value("keybinding/add", Qt::Key_2).value<Qt::Key>();
}

void Preferences::setKeyAdd(Qt::Key key) {
	QSettings().setValue("keybinding/add", key);
}

Qt::Key Preferences::keyPicker() {
	return QSettings().value("keybinding/picker", Qt::Key_3).value<Qt::Key>();
}

void Preferences::setKeyPicker(Qt::Key key) {
	QSettings().setValue("keybinding/picker", key);
}

Qt::Key Preferences::keyResize() {
	return QSettings().value("keybinding/resize", Qt::Key_4).value<Qt::Key>();
}

void Preferences::setKeyResize(Qt::Key key) {
	QSettings().setValue("keybinding/resize", key);
}

Qt::Key Preferences::key3DView() {
	return QSettings().value("keybinding/3dview", Qt::Key_F5).value<Qt::Key>();
}

void Preferences::setKey3DView(Qt::Key key) {
	QSettings().setValue("keybinding/3dview", key);
}

} // namespace eno
