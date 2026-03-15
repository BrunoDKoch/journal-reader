#pragma once
#include <QtWidgets>
#include "widgets/main_frame.hpp"

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private:
	MainFrame* _mainFrame;
};