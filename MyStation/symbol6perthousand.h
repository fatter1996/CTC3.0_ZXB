#ifndef SYMBOL6PERTHOUSAND_H
#define SYMBOL6PERTHOUSAND_H

#include <QObject>
#include <QPainter>

//千分之六标志
class Symbol6PerThousand : public QObject
{
    Q_OBJECT
public:
    explicit Symbol6PerThousand(QObject *parent = nullptr);

signals:

public:
    QPoint pCenterConst; //设备中心点，不被修改的，缩放及偏移使用
    bool   bRight;//是右，否左
    bool m_bVisible;//是否可见

public:
    //绘制(画笔，计时器，缩放倍数，偏移量)
    void Draw(QPainter *painter, long nElapsed, double nDiploid, QPoint offset, bool bOverturn = false);
};

#endif // SYMBOL6PERTHOUSAND_H
