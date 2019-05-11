#pragma once

#include <QDialog>
#include <QPushButton>
#include <QListWidget>
class LoadDlg : public QDialog
{
	Q_OBJECT
public:
	explicit LoadDlg(QWidget *parent = 0);
	~LoadDlg();
	void loadFile(QString fileName);

	QPushButton* _confirm,*_cancel;
	QListWidget *_battleList;
	QVector<QString> *_fenList;
	int _selected;
signals:

public slots:
	void slotConfirm();
	void slotCancel();
};


