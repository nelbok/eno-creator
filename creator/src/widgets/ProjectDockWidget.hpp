#pragma once

#include <QtWidgets/QDockWidget>

class QLineEdit;

namespace eno {
class Core;

class ProjectDockWidget : public QDockWidget {
	Q_OBJECT

public:
	ProjectDockWidget(QWidget* parent = 0);
	virtual ~ProjectDockWidget() = default;

	void init(Core* core);

private slots:
	void updateForm();

private:
	Core* _core{ nullptr };

	QLineEdit* _name{ nullptr };

signals:
	void showMessage(const QString& message);
};
} // namespace eno
