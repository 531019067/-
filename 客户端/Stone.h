#ifndef STONE_H
#define STONE_H

#include <QRect>
#include <QPainter>
#include <QTextCodec>
//const int PIECE_KING = 0;
//const int PIECE_ADVISOR = 1;
//const int PIECE_BISHOP = 2;
//const int PIECE_KNIGHT = 3;
//const int PIECE_ROOK = 4;
//const int PIECE_CANNON = 5;
//const int PIECE_PAWN = 6;
class Stone
{
public:
    Stone();
    ~Stone();

	enum TPYE{KING,ADVISOR,BISHOP,KNIGHT,ROOK,CANNON,PAWN};

    void init(int id);

    int _row;
    int _col;
	TPYE _type;
    bool _dead;
    bool _red;
    int _id;
	QString bmpDir();//位图资源地址
    QString name();

    void rotate();
};

#endif // STONE_H
