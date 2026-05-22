#pragma once
#include <QtWidgets>
#include <QMenuBar>
#include <QMenu>
#include <QCoreApplication>
#include "widgets/main_frame.hpp"

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private:
	MainFrame* _mainFrame;
	QMenu* _fileMenu;
	QMenu* _editMenu;
};