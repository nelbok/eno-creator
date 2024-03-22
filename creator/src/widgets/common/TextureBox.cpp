#include "TextureBox.hpp"

#include <eno/data/Project.hpp>

namespace eno {
TextureBox::TextureBox(QWidget* parent)
	: QComboBox(parent) {}

void TextureBox::init(Project* project) {
	assert(project);
	_project = project;
	connect(project, &Project::texturesUpdated, this, &TextureBox::updateTextures);
	connect(this, &TextureBox::currentIndexChanged, this, [this](int index) {
		emit currentTextureChanged(itemData(index).value<Texture*>());
	});
	updateTextures(project->textures());
}

void TextureBox::setCurrentTexture(Texture* current) {
	_current = current;
	if (current)
		setCurrentIndex(findData(QVariant::fromValue(current)));
	else
		setCurrentIndex(0);
}

void TextureBox::updateTextures(const QList<Texture*>& textures) {
	blockSignals(true);
	clear();

	// Default value
	addItem("None");
	setCurrentIndex(0);

	for (auto* texture : textures) {
		disconnect(texture, &Texture::nameUpdated, this, &TextureBox::updateTextureName);
		addItem(texture->name(), QVariant::fromValue(texture));
		connect(texture, &Texture::nameUpdated, this, &TextureBox::updateTextureName);
	}
	if (_current)
		setCurrentIndex(findData(QVariant::fromValue(_current)));
	blockSignals(false);
}

void TextureBox::updateTextureName() {
	assert(sender());
	auto* texture = qobject_cast<Texture*>(sender());
	assert(texture);
	int index = findData(QVariant::fromValue(texture));
	assert(index != -1);
	setItemText(index, texture->name());
}

} // namespace eno
