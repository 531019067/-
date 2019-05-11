#pragma once
#include <vector>
#include "ChessParameter.h"
#include "Stone.h"
#include "Step.h"
using namespace std;
/*
棋盘为16*16数组，
棋子类型编号从0到6,帅0，仕1，相2，马3，车4，炮5，兵6
棋子序号从0到31，0到15表示红子，16到31表示黑子。
每方的棋子顺序依次是：帅仕仕相相马马车车炮炮兵兵兵兵兵(将士士象象马马车车炮炮卒卒卒卒卒)

着法用整数表示
mv=sqSrc+sqDst*256,mv 代表走法，mv % 256 就是起始格子的编号，mv / 256 就是目标格子的编号。
*/
struct MoveStep
{
	int mv;
	uint8_t killid;
	bool isCheck;
	MoveStep(int mv=0,uint8_t killid=0,bool isCheck=false) {	}
};
class ChessSituation
{
	const uint8_t cBlack = 16;
	const uint8_t cRed = 8;
	const QString cInitSituation = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 0 1";
	const QString cStoneType= "KABNRCP";
public:
	ChessSituation();
	int getIndexFromRowCol(int row, int col);
	void move(int sqSrc,int sqDst);
	void move(int srcRow, int srcCol, int dstRow, int dstCol);
	void back();
	int getSqX(int sq);//获得格子横坐标
	int getSqY(int sq);//获得格子纵坐标


	//FEN串处理
	int getStoneType(char stone);
	void loadFromFen(QString Fen);
	QString createFen();
private:
	char *_board;//指向一个256的数组
	QVector<MoveStep> *_moveHistory;//步法历史表
	bool _bRedTurn;

};