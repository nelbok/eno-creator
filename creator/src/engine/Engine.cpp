#include "Engine.hpp"

#include <QtWidgets/QBoxLayout>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DExtras/QFirstPersonCameraController>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QDirectionalLight>

#include <eno/data/Material.hpp>
#include <eno/data/Project.hpp>
#include <eno/tools/Utils.hpp>

#include "CuboidMesh.hpp"

namespace eno {
Engine::Engine(QWidget* parent)
	: QWidget(parent) {}

void Engine::init(const Project* project) {
	setMinimumSize(1280, 780);
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

	for (auto* material : project->materials()) {
		auto* mesh = new CuboidMesh();
		mesh->init(project->scene(), material);

		auto* def = new Qt3DCore::QEntity(_root);
		def->addComponent(mesh);
		def->addComponent(createMaterialComponent(material));
		def->addComponent(new Qt3DCore::QTransform(_root));
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
	auto* light = new Qt3DRender::QDirectionalLight(entity);
	light->setColor("white");
	light->setIntensity(1);
	light->setWorldDirection({ 1.0f, -0.7f, -1.0f });
	entity->addComponent(light);
}

Qt3DCore::QComponent* Engine::createMaterialComponent(Material* mat) {
	assert(mat);

	auto* material = new Qt3DExtras::QDiffuseSpecularMaterial(_root);
	material->setDiffuse(mat->diffuse());
	return material;
}

} // namespace eno
