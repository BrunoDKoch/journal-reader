#include "widgets/main_window.hpp"


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
	_mainFrame = new MainFrame(this);
	setCentralWidget(_mainFrame);
}

MainWindow::~MainWindow() {
	_mainFrame = nullptr;
}