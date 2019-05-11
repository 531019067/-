#ifndef BOARD_H
#define BOARD_H

#include <QFrame>
#include "Stone.h"
#include "Step.h"
#include "ChessParameter.h"
#include "ChessSituation.h"
#include <QMessageBox>
#include <QVector>

class Board : public QFrame//棋盘类
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0);
    ~Board();

	const QString RedOrder = QString::fromLocal8Bit("九八七六五四三二一");
	const char BlackOrder[9] = { '1','2','3','4','5','6','7','8','9' };
    /* game data */
    Stone _s[32];//32个棋子
	ChessSituation _situation;//棋盘局面
    int _r;//棋子的半径
	int _d;
    QPoint _off;//棋盘左上角到第一个棋子中心点的偏移
	QPoint _pos;//棋盘左上角到窗体左上角的距离。
    bool _bRedSide;//是否执红方棋子
	
	QPixmap stone;
    QVector<Step*> _steps;//每一步棋的记录

    /* game status */
    int _selectid;//被选中的目标
    bool _bRedTurn;//当前是否轮到红方
    void init(bool bRedSide);//初始化摆棋，bRedSide决定红方是否在下方


	//FEN串处理
	int getType(char stone);
	void load(QString fen);
    /* draw functions绘制函数*/
    void paintEvent(QPaintEvent *);
	void drawPlate(QPainter& p); 
    void drawStone(QPainter& p);
    void drawStone(QPainter& p, int id);
	void drawSelected(QPainter &p, int id);

    /* function for coordinate 坐标转换*/
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
	void recordAddOrder(QString &record,bool stoneColor, int i);
	QString createRecord(Step * step);
    /* move stone */
    void mouseReleaseEvent(QMouseEvent *);
    void click(QPoint pt);
    virtual void click(int id, int row, int col);
    void trySelectStone(int id);
    void tryMoveStone(int killid, int row, int col);
    void actualMoveStone(int moveid, int killid, int row, int col);
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

	/*禁手判断*/
	void traverseNextStep();

	

signals:
	void sigMove(QString str);
	void sigBack();
public slots:
	void slotNew();
    void slotBack();
	void slotSave();
	void slotLoad();

};

#endif // BOARD_H
