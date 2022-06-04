#pragma once

#include <QObject>

#include <QColor>

namespace eno {

class Material : public QObject {
	Q_OBJECT

public:
	Material(QObject* parent = nullptr);
	virtual ~Material() = default;

	const QString& name() const {
		return _name;
	}

	void setName(const QString& name) {
		_name = name;
	}

	const QColor& diffuse() const {
		return _diffuse;
	}

	void setDiffuse(const QColor& color) {
		_diffuse = color;
	}

private:
	QString _name;
	QColor _diffuse{};

signals:

};
} // namespace eno
