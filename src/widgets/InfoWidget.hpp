#pragma once

#include <QWidget>

class QLabel;

namespace eno {
class Data;
class GraphicsView;

class InfoWidget : public QWidget {
	Q_OBJECT

public:
	InfoWidget(QWidget* parent = 0);
	virtual ~InfoWidget() = default;

	void init(Data* data, GraphicsView* graphicsView);

private slots:
	void updateData();

private:
	Data* _data{ nullptr };
	GraphicsView* _graphicsView{ nullptr };
	QLabel* _label{ nullptr };
};
} // namespace eno
