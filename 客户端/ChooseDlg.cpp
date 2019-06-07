 #include "ChooseDlg.h"
#include <QVBoxLayout>

ChooseDlg::ChooseDlg(QWidget *parent) : QDialog(parent)
{
	QPixmap pixmap = QPixmap(".\\Resources\\background.jpg").scaled(this->size());
	QPalette  palette(this->palette());
	palette.setBrush(QPalette::Background, QBrush(pixmap));
	this->setPalette(palette);

    QVBoxLayout* lay = new QVBoxLayout(this);
    lay->addWidget(_buttons[0] = new QPushButton(QString::fromLocal8Bit("人机对战")));
    lay->addWidget(_buttons[1] = new QPushButton(QString::fromLocal8Bit("人人对战")));
    lay->addWidget(_buttons[2] = new QPushButton(QString::fromLocal8Bit("在线对弈")));
   // lay->addWidget(_buttons[3] = new QPushButton(QString::fromLocal8Bit("网络对战(客户端)")));

    for(int i=0; i<3; ++i)
    {
        connect(_buttons[i], SIGNAL(clicked()), this, SLOT(slotClicked()));
    }
}

void ChooseDlg::slotClicked()
{
    QObject* s = sender();
    for(int i=0; i<4; ++i)
    {
        if(_buttons[i] == s)
        {
            this->_selected = i;
            break;
        }
    }
    accept();
}

ChooseDlg::~ChooseDlg()
{

}

