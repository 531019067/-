#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ChessClient.h"

class ChessClient : public QMainWindow
{
	Q_OBJECT

public:
	ChessClient(QWidget *parent = Q_NULLPTR);

private:
	Ui::ChessClientClass ui;
};
