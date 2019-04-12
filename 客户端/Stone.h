#ifndef STONE_H
#define STONE_H

#include <QRect>
#include <QPainter>
#include <QTextCodec>
class Stone
{
public:
    Stone();
    ~Stone();

    enum TYPE{CHE, MA, PAO, BING, JIANG, SHI, XIANG};

    void init(int id);

    int _row;
    int _col;
    TYPE _type;
    bool _dead;
    bool _red;
    int _id;
	QString bmpDir();//位图资源地址
    QString name();

    void rotate();
};

#endif // STONE_H
