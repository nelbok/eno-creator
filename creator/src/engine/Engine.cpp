#include "Engine.hpp"

#include <QtWidgets/QBoxLayout>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DExtras/QFirstPersonCameraController>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QTextureMaterial>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QDirectionalLight>
#include <Qt3DRender/QTexture>

#include <eno/data/Material.hpp>
#include <eno/data/Project.hpp>

#include "CuboidMesh.hpp"
#include "CuboidTexture.hpp"

namespace eno {
Engine::Engine(QWidget* parent)
	: QWidget(parent) {}

Engine::~Engine() {
	_view->deleteLater();
	_root->deleteLater();
}

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

	const auto& materials = project->materials();
	for (auto* material : materials) {
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

	// Fallback to a normal and ugly texture material...
	if (mat->texture()) {
		auto* textureImage = new CuboidTexture(_root);
		auto* texture = new Qt3DRender::QTexture2D(_root);
		textureImage->init(mat->texture());
		texture->addTextureImage(textureImage);

		auto* material = new Qt3DExtras::QTextureMaterial(_root);
		material->setTexture(texture);
		return material;
	}

	// Seems like it isn't possible anymore to give a QAbstractTexture to the material through the diffuse canal
	// Shame. Result: The material will be black.
	auto* material = new Qt3DExtras::QDiffuseSpecularMaterial(_root);
	material->setDiffuse(mat->diffuse());
	return material;
}

} // namespace eno
