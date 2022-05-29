#include "ComboBoxWidget.hpp"

#include <QBoxLayout>
#include <QLabel>
#include <QComboBox>

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

	connect(_comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
		this->currentItemChanged(this->_comboBox->itemData(index));
	});
}

void ComboBoxTool::addItem(const QString& text, const QVariant& data) {
	assert(_comboBox);
	_comboBox->addItem(text, data);
}

void ComboBoxTool::setCurrentItem(const QVariant& data) {
	auto index = _comboBox->findData(data);
	assert(index != -1);
	_comboBox->setCurrentIndex(index);
}

} // namespace eno
