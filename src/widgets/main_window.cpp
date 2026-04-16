#include "widgets/main_window.hpp"


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
	_mainFrame = new MainFrame(this);
	setCentralWidget(_mainFrame);
	setWindowIcon(QApplication::style()->standardIcon(QStyle::SP_ComputerIcon));
}

MainWindow::~MainWindow() {
	_mainFrame = nullptr;
}