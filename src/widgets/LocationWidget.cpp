#include "LocationWidget.hpp"

#include <QApplication>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

namespace eno {
LocationWidget::LocationWidget(QWidget* parent)
	: QWidget(parent) {}

void LocationWidget::init() {
	_lineEdit = new QLineEdit(this);
	auto* browse = new QPushButton("...", this);
	browse->setFixedWidth(browse->height());

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->addWidget(_lineEdit);
	layout->addWidget(browse);
	layout->setContentsMargins({ 0, 0, 0, 0 });
	layout->setSpacing(0);

	connect(_lineEdit, &QLineEdit::textChanged, this, &LocationWidget::locationChanged);
	connect(browse, &QPushButton::clicked, [this]() {
		const auto& dir = QFileDialog::getExistingDirectory(qApp->activeWindow(), qApp->applicationName() + " - Choose", location());
		if (dir.isEmpty())
			return;
		setLocation(dir);
	});
}

QString LocationWidget::location() const {
	return _lineEdit->text();
}

void LocationWidget::setLocation(const QString& location) {
	_lineEdit->setText(location);
}

} // namespace eno
