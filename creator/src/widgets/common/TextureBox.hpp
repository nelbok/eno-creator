#pragma once

#include <QtWidgets/QComboBox>

namespace eno {
class Project;
class Texture;

class TextureBox : public QComboBox {
	Q_OBJECT

public:
	TextureBox(QWidget* parent = 0);
	virtual ~TextureBox() = default;

	void init(Project* project);
	void setCurrentTexture(Texture* current);

private slots:
	void updateTextures(const QList<Texture*>& textures);
	void updateTextureName();

private:
	using QComboBox::QComboBox;
	using QComboBox::clear;
	using QComboBox::addItem;
	using QComboBox::setCurrentIndex;
	using QComboBox::setItemText;

private:
	Project* _project{ nullptr };
	Texture* _current{ nullptr };

signals:
	void currentTextureChanged(Texture* current);
};
} // namespace eno
