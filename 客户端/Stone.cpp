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
		case CHE:
			return ".\\Resources\\RCHE";
		case MA:
			return ".\\Resources\\RMA";
		case PAO:
			return ".\\Resources\\RPAO";
		case BING:
			return ".\\Resources\\RBING";
		case JIANG:
			return ".\\Resources\\RJIANG";
		case SHI:
			return ".\\Resources\\RSHI";
		case XIANG:
			return ".\\Resources\\RXIANG";
		}
	}
	else
	{
		switch (this->_type)
		{
		case CHE:
			return ".\\Resources\\BCHE";
		case MA:
			return ".\\Resources\\BMA";
		case PAO:
			return ".\\Resources\\BPAO";
		case BING:
			return ".\\Resources\\BBING";
		case JIANG:
			return ".\\Resources\\BJIANG";
		case SHI:
			return ".\\Resources\\BSHI";
		case XIANG:
			return ".\\Resources\\BXIANG";
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
		case CHE:
			return QString::fromLocal8Bit("車");
		case MA:
			return QString::fromLocal8Bit("馬");
		case PAO:
			return QString::fromLocal8Bit("炮");
		case BING:
			return QString::fromLocal8Bit("兵");
		case JIANG:
			return QString::fromLocal8Bit("帅");
		case SHI:
			return QString::fromLocal8Bit("士");
		case XIANG:
			return QString::fromLocal8Bit("相");
		}
	}
	else
	{
		switch (this->_type)
		{
		case CHE:
			return QString::fromLocal8Bit("車");
		case MA:
			return QString::fromLocal8Bit("馬");
		case PAO:
			return QString::fromLocal8Bit("炮");
		case BING:
			return QString::fromLocal8Bit("卒");
		case JIANG:
			return QString::fromLocal8Bit("将");
		case SHI:
			return QString::fromLocal8Bit("仕");
		case XIANG:
			return QString::fromLocal8Bit("象");
		}
	}
	return QString::fromLocal8Bit("错误");
}


void Stone::init(int id)//根据ID决定棋子类型，初始行列
{
    struct {
        int row, col;
        Stone::TYPE type;
    } pos[16] = {
    {0, 0, Stone::CHE},
    {0, 1, Stone::MA},
    {0, 2, Stone::XIANG},
    {0, 3, Stone::SHI},
    {0, 4, Stone::JIANG},
    {0, 5, Stone::SHI},
    {0, 6, Stone::XIANG},
    {0, 7, Stone::MA},
    {0, 8, Stone::CHE},

    {2, 1, Stone::PAO},
    {2, 7, Stone::PAO},
    {3, 0, Stone::BING},
    {3, 2, Stone::BING},
    {3, 4, Stone::BING},
    {3, 6, Stone::BING},
    {3, 8, Stone::BING},
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
