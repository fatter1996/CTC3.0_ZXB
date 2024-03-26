#ifndef TEXTANNOTATION_H
#define TEXTANNOTATION_H


#include <QObject>
#include <QPainter>
#include <QFontDialog>
#include <QColorDialog>

#define MIN_FONT_SIZE       1
#define MAX_FONT_SIZE       100

class TextAnnotation: public QObject
{
    Q_OBJECT
public:
    explicit TextAnnotation(QObject *parent = nullptr);
public:
    void Init(QRectF mRect, QString strtext, double nDiploid);
    void SetRect(QRectF rect);
    bool GetMousePoint(QPoint p);
    void SetTextFont();
    void SetTextcolor(int ntype);
    void Draw(QPainter *painter, long nElapsed, double nDiploid,QPoint offset,int type);
    int GetFontSizeByRect(QPainter *painter, QRectF rect, QString text);
public:
    QString strAnnotation;
    QRectF m_Rect; //控件显示区域
    QRectF m_Rectcont;
    QColor textcolor;
    QColor blackcolor;
    QFont font;
    double m_nDiploid = 1.0f;
    bool isShow = true;
};

#endif // TEXTANNOTATION_H
