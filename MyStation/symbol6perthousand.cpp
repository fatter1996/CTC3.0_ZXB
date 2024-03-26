#include "symbol6perthousand.h"
#include <QTextCodec>

Symbol6PerThousand::Symbol6PerThousand(QObject *parent) : QObject(parent)
{
    pCenterConst.setX(-100.0);
    pCenterConst.setY(-100.0);
    bRight = true;
    m_bVisible = true;
}

void Symbol6PerThousand::Draw(QPainter *painter, long nElapsed, double nDiploid, QPoint offset, bool bOverturn)
{
    if(!m_bVisible)
        return;
    bool right = bRight;
    QPoint pt = pCenterConst;
    if(bOverturn)
    {
        right = !right;
        pt = offset;
    }
    QPoint p1, p2, p3, p4, p5;
    int arrowLen = 4;//箭头长度
    QPoint pnt = QPoint(pt.x()*nDiploid, pt.y()*nDiploid);

    //字体（显示规范标准字体）
    int fontHigh = 12;
    //字体
    QFont font;
    font.setFamily("宋体");
    font.setPointSize(fontHigh*nDiploid);//字号
    font.setItalic(false);//斜体
    font.setBold(false);//加粗
    painter->setFont(font);//设置字体
    //画笔
    QPen pen1;
    pen1.setWidth(1*nDiploid);
    pen1.setColor(Qt::white);
    pen1.setStyle(Qt::SolidLine);
    painter->setPen(pen1);
    //不设置渲染器，否则绘制1px会显示2px！
    painter->setRenderHint(QPainter::Antialiasing,false);
    //文本
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QString strTxt = codec->toUnicode("6‰");
    QFontMetrics fm(font);
    QRect recsTxtSize = fm.boundingRect(strTxt);
    QRect textRect = QRect(pnt.x(), pnt.y(), recsTxtSize.width(), recsTxtSize.height());
    //点位坐标
    p1 = QPoint(textRect.left(), textRect.bottom());
    //显示6‰
    painter->drawText(textRect,Qt::AlignHCenter|Qt::AlignVCenter,strTxt);

    if(right) //朝右"-\"
    {
        p2 = QPoint(p1.x() + (recsTxtSize.right()-recsTxtSize.left()), p1.y());
        p3 = QPoint(p2.x() + (recsTxtSize.right()-recsTxtSize.left())/3, p2.y()+(recsTxtSize.right() - recsTxtSize.left())/3);
        p4 = QPoint(p3.x(), p3.y() - arrowLen * nDiploid);
        p5 = QPoint(p3.x() - arrowLen * nDiploid, p3.y());
        painter->drawLine(p1,p2);
        painter->drawLine(p2,p3);
        painter->drawLine(p3,p4);
        painter->drawLine(p3,p5);
    }
    else //朝左"/-"
    {
        p2 = QPoint(p1.x() + (recsTxtSize.right()-recsTxtSize.left()), p1.y());
        p3 = QPoint(p1.x() - (recsTxtSize.right()-recsTxtSize.left())/3, p1.y()+(recsTxtSize.right() - recsTxtSize.left())/3);
        p4 = QPoint(p3.x(), p3.y() - arrowLen * nDiploid);
        p5 = QPoint(p3.x() + arrowLen * nDiploid, p3.y());
        painter->drawLine(p1,p2);
        painter->drawLine(p1,p3);
        painter->drawLine(p3,p4);
        painter->drawLine(p3,p5);
    }
}
