#ifndef TEXTANALYSIS_H
#define TEXTANALYSIS_H
#pragma execution_character_set("utf-8")
#include <QByteArray>
#include <QObject>
#include <QDateTime>
#include "GlobalHeaders/GlobalFuntion.h"
#include "MyStation/mystation.h"
#include "qwidget.h"
#include <QWidget>

struct TextAnalysisInfo{
    int index = -1;
    int color = -1;
    int timeLength = -1;
    QString text;
};

class TextAnalysis : public QObject
{
    Q_OBJECT
public:
    TextAnalysis(QObject *parent = nullptr);
    ~TextAnalysis();

    void TextAnalysisInfoInit();
    void setStratTime();
    //TextFrame* TextFrameUnpack(QByteArray dataArr);
    void sendTextAnalysisMsg(int index, QPoint point, QString dev = "");
    //QByteArray TextFramePacket(TextFrame* frame);
    //Dialogue* TextFrameToDialogue(TextFrame* frame);

signals:
    void TextFramePacket(TextFrame frame);
private:
    QDateTime curTime;
    QVector<TextAnalysisInfo> vecTextAnalysisInfo;
};

#endif // TEXTANALYSIS_H
