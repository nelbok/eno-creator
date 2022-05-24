#pragma once

#include <QColor>
#include <QMap>
#include <QObject>
#include <QVector2D>
#include <QVector3D>

namespace eno {
using MapItem = QMap<QVector3D, QColor>;

class Data : public QObject {
	Q_OBJECT

public:
	Data(QObject* parent = nullptr);
	virtual ~Data() = default;

	void reset();

	// clang-format off
	const QVector2D& min() const { return _min; }
	const QVector2D& max() const { return _max; }
	void setMin(const QVector2D& min);
	void setMax(const QVector2D& max);
	// clang-format on

	void addItem(const QVector3D& pos, const QColor& color);
	void removeItem(const QVector3D& pos);
	bool findItem(const QVector3D& pos) const;
	const QColor colorAt(const QVector3D& pos) const;

	MapItem::const_key_value_iterator begin() const;
	MapItem::const_key_value_iterator end() const;

private:
	void updateScene();

	QVector2D _min{};
	QVector2D _max{};

	MapItem _scene{};

signals:
	void rectUpdated();
	void sceneUpdated();
};
} // namespace eno
