#include <QApplication>
#include "LobbyClient.h"
#include "LoginGUI.h"
#include "ChooseDlg.h"
#include "MainWnd.h"
#include <QTime>
#include <string>

using namespace std;
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
	string a;
    QTime t = QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);

	ChooseDlg  dlg;
	dlg.setWindowTitle(QString::fromLocal8Bit("ÖÐ¹úÏóÆå"));
	dlg.resize(200, 200);

	if (dlg.exec() != QDialog::Accepted)
		return 0;
	if (dlg._selected == 0 || dlg._selected == 1)
	{
		MainWnd *wnd=new MainWnd(dlg._selected);
		wnd->show();
	}
	else
	{
		Lobby *lobby=new Lobby;
		lobby->init();
		lobby->show();

		if (lobby->login() == false)
			return 0;
	}



    return app.exec();
}
