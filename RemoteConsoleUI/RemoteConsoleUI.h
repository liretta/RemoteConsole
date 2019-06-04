#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_RemoteConsoleUI.h"

class RemoteConsoleUI : public QMainWindow
{
	Q_OBJECT

public:
	RemoteConsoleUI(QWidget *parent = Q_NULLPTR);

private:
	Ui::RemoteConsoleUIClass ui;
};
