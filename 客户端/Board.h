#ifndef BOARD_H
#define BOARD_H

#include <QFrame>
#include "Stone.h"
#include "Step.h"
#include <QMessageBox>
#include <QVector>

class Board : public QFrame//������
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0);
    ~Board();

    /* game data */
    Stone _s[32];//32������
    int _r;//���ӵİ뾶
	int _d;
    QPoint _off;//���̵�ƫ��
	QPoint _pos;
    bool _bSide;//��߻��Ǻڱ�
	
	QPixmap stone;
    QVector<Step*> _steps;//ÿһ����ļ�¼

    /* game status */
    int _selectid;
    bool _bRedTurn;
    void init(bool bRedSide);//��ʼ�����壬bRedSide�����췽�Ƿ����·�

    /* draw functions���ƺ���*/
    void paintEvent(QPaintEvent *);
	void drawPlate(QPainter& p); 
    void drawStone(QPainter& p);
    void drawStone(QPainter& p, int id);
	void drawSelected(QPainter &p, int id);

    /* function for coordinate ����ת��*/
    QPoint center(int row, int col);
    QPoint center(int id);
    QPoint topLeft(int row, int col);
    QPoint topLeft(int id);
    QRect cell(int row, int col);
    QRect cell(int id);

    bool getClickRowCol(QPoint pt, int& row, int& col);

    /* help function */
    QString name(int id);
    bool red(int id);
    bool sameColor(int id1, int id2);
    int getStoneId(int row, int col);
    void killStone(int id);
    void reliveStone(int id);
    void moveStone(int moveid, int row, int col);
    bool isDead(int id);

    /* move stone */
    void mouseReleaseEvent(QMouseEvent *);
    void click(QPoint pt);
    virtual void click(int id, int row, int col);
    void trySelectStone(int id);
    void tryMoveStone(int killid, int row, int col);
    void moveStone(int moveid, int killid, int row, int col);
    void saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps);
    void backOne();
    void back(Step* step);
    virtual void back();

    /* rule */
    bool canMove(int moveid, int killid, int row, int col);
    bool canMoveChe(int moveid, int killid, int row, int col);
    bool canMoveMa(int moveid, int killid, int row, int col);
    bool canMovePao(int moveid, int killid, int row, int col);
    bool canMoveBing(int moveid, int killid, int row, int col);
    bool canMoveJiang(int moveid, int killid, int row, int col);
    bool canMoveShi(int moveid, int killid, int row, int col);
    bool canMoveXiang(int moveid, int killid, int row, int col);

    bool canSelect(int id);

    /* rule helper */
    int getStoneCountAtLine(int row1, int col1, int row2, int col2);
    int relation(int row1, int col1, int row, int col);
    bool isBottomSide(int id);

signals:

public slots:
    void slotBack();

};

#endif // BOARD_H
