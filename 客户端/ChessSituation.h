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
struct MoveStruct
{
	int mv;
	uint8_t killid;
	bool isCheck;
	MoveStruct(int mv=0,uint8_t killid=0,bool isCheck=false):mv(mv),killid(killid),isCheck(isCheck) {	}
};
class ChessSituation
{
public:
	ChessSituation(bool direction=true);
	int getIndexFromRowCol(int row, int col);
	void move(int sqSrc,int sqDst);
	void move(int srcRow, int srcCol, int dstRow, int dstCol);
	void back();
	int getSqX(int sq);//获得格子横坐标
	int getSqY(int sq);//获得格子纵坐标
	bool sameColor(int src, int dst);//如果颜色相同返回true
	bool awayHalf(int sq);//是否过楚河汉界，已过河返回true，未过河返回false
	int flipSquare(int sq);//翻转棋盘
	void flipSquare();//翻转整个棋盘
	bool check();//判断是否被将军
	bool chase();//判断是否为捉
	bool mate();//判断是否为杀
	void addPiece(int sq, int pc); // 在棋盘第sq格子上放一枚棋子pc
	void delPiece(int sq);// // 从棋盘上sq格子上拿走一枚棋子
	int evaluate(void) const;//局面评估函数


	//FEN串处理
	int getStoneType(char stone);
	void loadFromFen(QString Fen);
	QString createFen();

public:
	const uint8_t cBlack = 16;
	const uint8_t cRed = 8;
	const QString cInitSituation = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 0 1";
	const QString cStoneType = "KABNRCP";
	enum TYPE { KING, ADVISOR, BISHOP, KNIGHT, ROOK, CANNON, PAWN };

	int _sqRKing, _sqBKing;//红黑方将军的位置
	char *_board;//指向一个256的数组
	QVector<MoveStruct*> _moveHistory;//步法历史表
	QVector<MoveStruct*> _searchTree;//博弈树
	char _isRedTurn;//0是红方棋子走，1是黑方棋子
	bool _direction;//棋盘的方向,默认为true红下黑上，为false时红上黑下
	int _redValue, _blackValue;//红黑方的子力


};