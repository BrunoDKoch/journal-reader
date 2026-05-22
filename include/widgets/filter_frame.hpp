#pragma once
#include <QtWidgets>
#include "enums/priority_enum.hpp"

class FilterFrame : public QFrame {
	Q_OBJECT
public:
	explicit FilterFrame(QWidget* parent = nullptr);
	~FilterFrame();

signals:
	void sinceDateSelected(const QString& since);
	void untilDateSelected(const QString& until);
	void update(const QStringList& filterStringList);

private slots:
	void selectSinceDate();
	void selectUntilDate();
	void selectPriority();
	void submitFilterString();

private:
	QLineEdit* _stringEdit;
	QPushButton* _submitButton;
	QComboBox* _prioritySelect;
	PriorityEnum _priority;
};