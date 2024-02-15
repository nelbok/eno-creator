#pragma once

#include <QtWidgets/QWidget>

class QComboBox;

namespace eno {

class ComboBoxTool : public QWidget {
	Q_OBJECT

public:
	ComboBoxTool(QWidget* parent = 0);
	virtual ~ComboBoxTool() = default;

	void init(const QString& icon);
	void addItem(const QString& text, const QVariant& data);

public slots:
	void setCurrentItem(const QVariant& data);

private:
	QComboBox* _comboBox{ nullptr };

signals:
	void currentItemChanged(const QVariant&);
};
} // namespace eno
