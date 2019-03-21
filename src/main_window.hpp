#pragma once

#include <QtWidgets>

class MainWindow : public QMainWindow
{
public:
	//  override the constructor
	explicit MainWindow(QWidget * parent = nullptr, Qt::WindowFlags flags = 0);
};
