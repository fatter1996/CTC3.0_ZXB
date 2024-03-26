#include "trainnumwnd.h"

TrainNumWnd::TrainNumWnd(QObject *parent) : QObject(parent)
{
    m_nPosCode = -1;
    pCenter.setX(-100.0);
    pCenter.setY(-100.0);
    m_RectWnd.setRect(0,0,0,0);
    m_bVisible = false;
    m_bShow = false;
    m_bVisibleRange = false;
}
//绘制(画笔，计时器，缩放倍数，偏移量)
void TrainNumWnd::Draw(QPainter *painter, long nElapsed, double nDiploid, double nDiploidCheci, QPoint offset)
{
    pCenter.setX(pCenterConst.x()*nDiploid+offset.x()*nDiploid);
    pCenter.setY(pCenterConst.y()*nDiploid+offset.y()*nDiploid);

    //范围
    int width = 60*nDiploid*nDiploidCheci;//宽
    int heigh = 24*nDiploid*nDiploidCheci;//高26
    m_RectWnd.setRect(pCenter.x()-width/2, pCenter.y()-heigh/2, width, heigh);

    //显示
    //无车次&&可见设置
    if((m_bVisible && m_bShow) || m_bVisibleRange)
    {
        QVector<qreal> dashes;
        qreal space = 5*nDiploid*nDiploidCheci;
        qreal line = 8*nDiploid*nDiploidCheci;
        qreal dot = 2*nDiploid*nDiploidCheci;
        dashes << line << space << dot << space;// << line << space << dot;
        //画笔
        QPen pen1;
        pen1.setWidth(1*nDiploid*nDiploidCheci);
        pen1.setColor(Qt::white);
        //pen1.setStyle(Qt::DashDotLine);
        pen1.setDashPattern(dashes);//设置自定义样式（点线）
        painter->setPen(pen1);
        //不设置渲染器，否则绘制1px会显示2px！
        painter->setRenderHint(QPainter::Antialiasing,false);
        //设置背景为透明
        painter->setBrush(Qt::NoBrush);
        //绘制矩形
        painter->drawRect(m_RectWnd);
    }
}
