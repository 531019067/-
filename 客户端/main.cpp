#include "ChessClient.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ChessClient w;
	w.show();
	return a.exec();
}
