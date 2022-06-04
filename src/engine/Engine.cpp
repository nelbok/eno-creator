#include "Engine.hpp"

#include <QBoxLayout>

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DExtras/QFirstPersonCameraController>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QPointLight>

#include "data/Scene.hpp"
#include "data/Material.hpp"
#include "Utils.hpp"

namespace eno {
Engine::Engine(QWidget* parent)
	: QWidget(parent) {}

void Engine::init(const Scene* scene) {
	setMinimumSize(1280, 780);
	setWindowIcon(QIcon(":/logo/logo.png"));
	setWindowModality(Qt::WindowModality::ApplicationModal);
	setAttribute(Qt::WA_DeleteOnClose, true);

	_view = new Qt3DExtras::Qt3DWindow();
	_view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
	QWidget* container = QWidget::createWindowContainer(_view);

	_root = new Qt3DCore::QEntity();
	_view->setRootEntity(_root);

	auto* layout = new QHBoxLayout(this);
	layout->addWidget(container);

	initCamera();
	initLight();
	initCube();

	for (const auto& item : *scene) {
		auto* transform = new Qt3DCore::QTransform(_root);
		transform->setTranslation(item.first);

		auto* def = new Qt3DCore::QEntity(_root);
		def->addComponent(_mesh);
		def->addComponent(getMaterialBy(item.second));
		def->addComponent(transform);
	}
}

void Engine::initCamera() {
	assert(_view);
	assert(_root);
	auto* camera = _view->camera();
	camera->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	camera->setPosition(QVector3D(0, 20, 20.0f));
	camera->setUpVector(QVector3D(0, 1, 0));
	camera->setViewCenter(QVector3D(0, 0, 0));

	auto* camController = new Qt3DExtras::QFirstPersonCameraController(_root);
	camController->setCamera(camera);
}

void Engine::initLight() {
	assert(_view);
	assert(_root);
	auto* entity = new Qt3DCore::QEntity(_root);
	auto* light = new Qt3DRender::QPointLight(entity);
	light->setColor("white");
	light->setIntensity(1);
	entity->addComponent(light);

	auto* transform = new Qt3DCore::QTransform(entity);
	transform->setTranslation(_view->camera()->position());
	entity->addComponent(transform);
}

void Engine::initCube() {
	// We need a better cube
	_mesh = new Qt3DExtras::QCuboidMesh(_root);
}

Qt3DCore::QComponent* Engine::getMaterialBy(Material* mat) {
	assert(mat);
	const auto it = _materials.find(mat);
	if (it != _materials.end()) {
		return it.value();
	}

	auto* material = new Qt3DExtras::QDiffuseSpecularMaterial(_root);
	material->setDiffuse(mat->diffuse());
	_materials.insert(mat, material);
	return material;
}

} // namespace eno
