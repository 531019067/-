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
	QLabel *_picture;
signals:
    void sigBack();

public slots:
};

#endif // CTRLPANEL_H
