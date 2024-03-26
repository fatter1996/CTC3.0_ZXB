#include "textannotation.h"
#include <qdebug.h>
#pragma execution_character_set("utf-8")

TextAnnotation::TextAnnotation(QObject *parent) : QObject(parent)
{
    this->setObjectName("TextAnnotation");
    this->installEventFilter(parent);
    //this->setMouseTracking(true);
    blackcolor=QColor(255, 255, 255);
    textcolor=Qt::red;
    font.setFamily("宋体");
    font.setPointSize(12);//字号
    font.setItalic(false);//斜体
}
void TextAnnotation::Init(QRectF mRect, QString strtext, double nDiploid)
{
    m_nDiploid = nDiploid;
    strAnnotation = strtext;
    QFontMetrics  fontMetrics(font);
    int x = mRect.x();
    int y = mRect.y();
    int w = fontMetrics.width(strAnnotation) + 8;
    int h = fontMetrics.height() + 8;
    mRect.setX(x);
    mRect.setY(y);
    mRect.setWidth(w);
    mRect.setHeight(h);
    //m_Rect.setLeft(fontMetrics.width(strAnnotation) + 8);
    //m_Rect.setTop(fontMetrics.height() + 8);
    SetRect(mRect);
}
void TextAnnotation::SetRect(QRectF rect)
{
    m_Rectcont.setX(rect.x() / m_nDiploid);
    m_Rectcont.setY(rect.y() / m_nDiploid);
    m_Rectcont.setWidth(rect.width());
    m_Rectcont.setHeight(rect.height());

    m_Rect.setX(rect.x());
    m_Rect.setY(rect.y());
    m_Rect.setWidth(rect.width() * m_nDiploid);
    m_Rect.setHeight(rect.height() * m_nDiploid);
}
bool TextAnnotation::GetMousePoint(QPoint p)
{
    return m_Rect.contains(p);
}

void TextAnnotation::SetTextFont()
{
    bool bFlag = true;
    font = QFontDialog::getFont(&bFlag);
    if(bFlag)
    {
        QFontMetrics  fontMetrics(font);
        int w = fontMetrics.width(strAnnotation) + 8;
        int h = fontMetrics.height() + 8;
        m_Rectcont.setWidth(w);
        m_Rectcont.setHeight(h);
    }
}
void TextAnnotation::SetTextcolor(int ntype)
{
    bool bFlag = true;
    QColorDialog mQColorDialog;
    //ui->textEdit->setEnabled(true);
    mQColorDialog.show();
    if(mQColorDialog.exec()==QColorDialog::Accepted)
    {
        QColor color = mQColorDialog.selectedColor();

        QString str = QString::number(color.red())+","+QString::number(color.green())+","+QString::number(color.blue());
        if(ntype==0)
        {
            QString str2 = QString::number(blackcolor.red())+","+QString::number(blackcolor.green())+","+QString::number(blackcolor.blue());
            //ui->textEdit->setStyleSheet(QString("QTextEdit{color:rgb(%1);background-color:rgb(%2);}").arg(str).arg(str2));
            textcolor=color;
        }
        else if(ntype==1)
        {
            QString str2 = QString::number(textcolor.red())+","+QString::number(textcolor.green())+","+QString::number(textcolor.blue());
           // ui->textEdit->setStyleSheet(QString("QTextEdit{background-color:rgb(%1);color:rgb(%2);}").arg(str).arg(str2));
            blackcolor=color;
        }
    }
//    ui->textEdit->setEnabled(false);
//    update();
}
void TextAnnotation::Draw(QPainter *painter, long nElapsed, double nDiploid, QPoint offset, int type)
{
    if(!isShow)
        return;

    m_nDiploid = nDiploid;
    m_Rect.setX(m_Rectcont.x() * nDiploid);
    m_Rect.setY(m_Rectcont.y() * nDiploid);
    m_Rect.setWidth(m_Rectcont.width() * nDiploid);
    m_Rect.setHeight(m_Rectcont.height() * nDiploid);
    QBrush newBrushBK;
    QPen pen; //画笔
    newBrushBK.setStyle(Qt::SolidPattern);
    newBrushBK.setColor(blackcolor);
    painter->setPen(pen);
    painter->setBrush(newBrushBK);
    //绘制矩形
    painter->drawRect(m_Rect);
    //绘制文本
    int size = GetFontSizeByRect(painter, m_Rect, strAnnotation);
    font.setPixelSize(size);
    painter->setFont(font);
    painter->setPen(textcolor);//设置画笔颜色backgroundRect, Qt::AlignCenter, text
    painter->drawText(m_Rect, Qt::AlignCenter, strAnnotation);
}

int TextAnnotation::GetFontSizeByRect(QPainter *painter, QRectF rect, QString text)
{
    QRectF rectangle(0, 0, rect.width() - 8, rect.height() - 8);
    QRectF boundingRect;
    QTextOption o;
    o.setWrapMode(QTextOption::WrapAnywhere);
    o.setAlignment(Qt::AlignVCenter);

    QFont font = painter->font();
    font.setFamily("楷体"); //字体
    for(int i = MIN_FONT_SIZE; i < MAX_FONT_SIZE; i++)
    {
        font.setPixelSize(i);
        painter->setFont(font);
        boundingRect = painter->boundingRect(rectangle, text, o);
        if(!rectangle.contains(boundingRect))
        {
            return (i - 1) == 0 ? MIN_FONT_SIZE : (i - 1);
        }
    }
}
