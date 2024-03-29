#include "ComboBoxTool.hpp"

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>

namespace eno {
ComboBoxTool::ComboBoxTool(QWidget* parent)
	: QWidget(parent) {}

void ComboBoxTool::init(const QString& icon) {
	_comboBox = new QComboBox(this);
	auto* label = new QLabel(this);

	label->setPixmap(QPixmap(icon));

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->addWidget(_comboBox);
	layout->addWidget(label);

	connect(_comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
		emit currentItemChanged(_comboBox->itemData(index));
	});
}

void ComboBoxTool::addItem(const QString& text, const QVariant& d) {
	assert(_comboBox);
	_comboBox->addItem(text, d);
}

void ComboBoxTool::setCurrentItem(const QVariant& d) {
	auto index = _comboBox->findData(d);
	assert(index != -1);
	_comboBox->setCurrentIndex(index);
}

} // namespace eno
