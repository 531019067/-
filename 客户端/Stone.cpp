#include "Stone.h"
#include <QDebug>

Stone::Stone()
{

}

Stone::~Stone()
{

}

QString Stone::bmpDir()
{
	if (_red)
	{
		switch (this->_type)
		{
		case ROOK://车
			return ".\\Resources\\RR";
		case KNIGHT://马
			return ".\\Resources\\RN";
		case CANNON://炮
			return ".\\Resources\\RC";
		case PAWN://兵
			return ".\\Resources\\RP";
		case KING://帅
			return ".\\Resources\\RK";
		case ADVISOR://士
			return ".\\Resources\\RA";
		case BISHOP://象
			return ".\\Resources\\RB";
		}
	}
	else
	{
		switch (this->_type)
		{
		case ROOK:
			return ".\\Resources\\BR";
		case KNIGHT:
			return ".\\Resources\\BN";
		case CANNON:
			return ".\\Resources\\BC";
		case PAWN:
			return ".\\Resources\\BP";
		case KING:
			return ".\\Resources\\BK";
		case ADVISOR:
			return ".\\Resources\\BA";
		case BISHOP:
			return ".\\Resources\\BB";
		}
	}
	return QString::fromLocal8Bit("错误");
}

QString Stone::name()
{
	if (_red)
	{
		switch (this->_type)
		{
		case ROOK:
			return QString::fromLocal8Bit("車");
		case KNIGHT:
			return QString::fromLocal8Bit("馬");
		case CANNON:
			return QString::fromLocal8Bit("炮");
		case PAWN:
			return QString::fromLocal8Bit("兵");
		case KING:
			return QString::fromLocal8Bit("帅");
		case ADVISOR:
			return QString::fromLocal8Bit("士");
		case BISHOP:
			return QString::fromLocal8Bit("相");
		}
	}
	else
	{
		switch (this->_type)
		{
		case ROOK:
			return QString::fromLocal8Bit("車");
		case KNIGHT:
			return QString::fromLocal8Bit("馬");
		case CANNON:
			return QString::fromLocal8Bit("炮");
		case PAWN:
			return QString::fromLocal8Bit("卒");
		case KING:
			return QString::fromLocal8Bit("将");
		case ADVISOR:
			return QString::fromLocal8Bit("仕");
		case BISHOP:
			return QString::fromLocal8Bit("象");
		}
	}
	return QString::fromLocal8Bit("错误");
}


void Stone::init(int id)//根据ID决定棋子类型，初始行列
{
	//每方的棋子顺序依次是：帅仕仕相相马马车车炮炮兵兵兵兵兵(将士士象象马马车车炮炮卒卒卒卒卒)
    struct {
        int row, col;
		Stone::TPYE type;
    } pos[16] = {
	{0, 4, Stone::KING},
	{0, 3, Stone::ADVISOR},
	{0, 5, Stone::ADVISOR},
	{0, 2, Stone::BISHOP},
	{0, 6, Stone::BISHOP},
	{0, 1, Stone::KNIGHT},
	{0, 7, Stone::KNIGHT},
    {0, 0, Stone::ROOK},
    {0, 8, Stone::ROOK},

    {2, 1, Stone::CANNON},
    {2, 7, Stone::CANNON},
    {3, 0, Stone::PAWN},
    {3, 2, Stone::PAWN},
    {3, 4, Stone::PAWN},
    {3, 6, Stone::PAWN},
    {3, 8, Stone::PAWN},
    };

    if(id < 16)
    {
        this->_col = pos[id].col;
        this->_row = pos[id].row;
        this->_type = pos[id].type;
    }
    else
    {
        this->_col = 8-pos[id-16].col;
        this->_row = 9-pos[id-16].row;
        this->_type = pos[id-16].type;
    }

    this->_dead = false;
    this->_red = id<16;
}

void Stone::rotate()//翻转棋子
{
    this->_col = 8-this->_col;
    this->_row = 9-this->_row;
}
