#ifndef TRAINNUMWND_H
#define TRAINNUMWND_H

#include <QObject>
#include <QPainter>

//车次窗（车次显示位置窗口）
class TrainNumWnd : public QObject
{
    Q_OBJECT
public:
    explicit TrainNumWnd(QObject *parent = nullptr);

signals:

public:
    QString m_strCheCi;//车次
    int  m_nPosCode;//显示位置的设备号
    QPointF pCenterConst; //设备中心点，不被修改的，缩放及偏移使用
    QPoint pCenter; //设备中心点，绘制的
    QRect m_RectWnd;//矩形框
    bool m_bVisible;//是否可见
    bool m_bShow;//是否系统设置可见
    bool m_bVisibleRange;//是否可见范围
    bool misqj=false;//车次窗位置 站内 区间

public:
    //绘制(画笔，计时器，缩放倍数，偏移量)
    void Draw(QPainter *painter, long nElapsed, double nDiploid, double nDiploidCheci, QPoint offset);
};

#endif // TRAINNUMWND_H
