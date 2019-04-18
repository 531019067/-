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
	Lobby lobby;
	lobby.init();
	lobby.show();
	
	if (lobby.login() == false)
		return 0;
	/*ChooseDlg  dlg;
	if(dlg.exec() != QDialog::Accepted)
		return 0;

	MainWnd wnd(dlg._selected);
	wnd.show();
*/



    return app.exec();
}
