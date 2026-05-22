#include "widgets/main_window.hpp"


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
	_mainFrame = new MainFrame(this);
	setCentralWidget(_mainFrame);
	setWindowIcon(QApplication::style()->standardIcon(QStyle::SP_ComputerIcon));
	setWindowTitle("Journal Reader");

	QMenuBar* topMenuBar = this->menuBar();

	_fileMenu = topMenuBar->addMenu(tr("&File"));
	QAction* exportAct = _fileMenu->addAction(tr("&Export Logs..."));
	exportAct->setShortcut(QKeySequence::Save);
	connect(exportAct, &QAction::triggered, _mainFrame, &MainFrame::exportLogs);

	_fileMenu->addSeparator();

	QAction* exitAct = _fileMenu->addAction(tr("E&xit"));
	exitAct->setShortcut(QKeySequence::Quit);
	connect(exitAct, &QAction::triggered, qApp, &QCoreApplication::quit);

	_editMenu = topMenuBar->addMenu(tr("&Edit"));
	QAction* copyAct = _editMenu->addAction(tr("&Copy Selected"));
	copyAct->setShortcut(QKeySequence::Copy);
	connect(copyAct, &QAction::triggered, _mainFrame, &MainFrame::copySelectedLogs);

	_editMenu->addSeparator();

	QAction* refreshAct = _editMenu->addAction(tr("&Refresh"));
	refreshAct->setShortcut(QKeySequence(Qt::Key_F5));
	connect(refreshAct, &QAction::triggered, _mainFrame, &MainFrame::reload);
}

MainWindow::~MainWindow() {
	_mainFrame = nullptr;
}