#ifndef CTRLPANEL_H
#define CTRLPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class CtrlPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CtrlPanel(QWidget *parent = 0);
    ~CtrlPanel();

    QPushButton* _back;
	QPushButton* _newBattle;
	QPushButton* _saveBattle;
	QPushButton* _loadBattle;
	QPushButton* _giveUp;
	QPushButton* _suePeace;
	QPushButton* _menu;

signals:
    void sigBack();
	void sigNew();
	void sigLoad();
	void sigSave();
	void sigGive();
	void sigPeace();
public slots:

};

#endif // CTRLPANEL_H
