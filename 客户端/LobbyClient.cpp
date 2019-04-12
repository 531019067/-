#include "LobbyClient.h"

Lobby::Lobby(QWidget *parent /* = 0 */):QMainWindow(parent)
{
	this->setWindowTitle(QString::fromLocal8Bit("游戏大厅"));
	setMinimumSize(500,500);
	_socket = new QTcpSocket(this);
	//_userInfo = new UserInformation;
	_socket->connectToHost(QHostAddress("127.0.0.1"), 9898);
	if (_socket->waitForConnected() == false)
		QMessageBox::information(this, "tips", QString::fromLocal8Bit("服务器无法连接"));

}
Lobby::~Lobby()
{

}
bool Lobby::login()
{
	LoginGUI *login = new LoginGUI(_socket);
	if (login->exec() != QDialog::Accepted)
		return false;
	
	_userInfo = login->getUserInfo();
	login->disconnect(_socket, SIGNAL(readyRead()), login, SLOT(slotDataArrive()));
	login->close();
	delete login;
	return true;
}
void Lobby::init()
{


}

