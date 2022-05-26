#pragma once

#include <QColor>
#include <QMap>
#include <QObject>
#include <QVector2D>
#include <QVector3D>

namespace eno {
//  Data Graphical Representation
//
//  Y
//  |  Z
//  | /       = MapItem
//  |/
//  +-----X
// 0
//
//  Min / Max
//     +-------+ max (X,Z)
//    /       /
//   /       /
//  +-------+
// 0 = min (X,Z)

using MapItem = QMap<QVector3D, QColor>;

class Data : public QObject {
	Q_OBJECT

public:
	Data(QObject* parent = nullptr);
	virtual ~Data() = default;

	void reset();

	// clang-format off
	QString projectName() const;
	const QString& filePath() const { return _filePath; }
	bool isModified() const { return _isModified; }
	const QVector2D& min() const { return _min; }
	const QVector2D& max() const { return _max; }
	void setFilePath(const QString& path);
	void setIsModified(bool isModifed);
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

	QString _filePath{};
	bool _isModified{ false };

	QVector2D _min{};
	QVector2D _max{};

	MapItem _scene{};

signals:
	void filePathUpdated();
	void isModifiedUpdated();
	void rectUpdated();
	void sceneUpdated();
};
} // namespace eno
