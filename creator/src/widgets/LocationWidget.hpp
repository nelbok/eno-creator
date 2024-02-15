#pragma once

#include <QtWidgets/QWidget>

class QLineEdit;

namespace eno {

class LocationWidget : public QWidget {
	Q_OBJECT

public:
	LocationWidget(QWidget* parent = 0);
	virtual ~LocationWidget() = default;

	void init();

	QString location() const;
	void setLocation(const QString& location);

private:
	QLineEdit* _lineEdit{ nullptr };

signals:
	void locationChanged(const QString&);
};
} // namespace eno
