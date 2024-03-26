#include "TextAnalysis.h"
#include <QDebug>
#include <QTextCodec>
#pragma execution_character_set("utf-8")

TextAnalysis::TextAnalysis(QObject *parent)
{

}

TextAnalysis::~TextAnalysis()
{

}
void TextAnalysis::TextAnalysisInfoInit()
{
    QString fileName = QString("Data/MouseAction.txt");
    QString m_str;
    QString  getString;
    QFileInfo fileInfo(fileName);
    if (!fileInfo.isFile())
    {
        return;
    }
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    m_str = codec->toUnicode(file.readLine());

    while(m_str.left(4) != "####")
    {
        if(m_str.left(4) == "##>>")
        {
            m_str = codec->toUnicode(file.readLine());
            continue;
        }
        m_str.replace(";\r\n","");
        TextAnalysisInfo info;
        QStringList list = m_str.split(":");
        info.index = list.at(0).toInt();
        QStringList list2 = list.at(1).split("|");
        info.text = list2.at(0);
        info.color = list2.at(1).toInt();
        info.timeLength = list2.at(2).toInt();
        vecTextAnalysisInfo.append(info);
        m_str = codec->toUnicode(file.readLine());
    }
}

void TextAnalysis::setStratTime()
{
    curTime = QDateTime::currentDateTime();
}
/*
TextFrame* TextAnalysis::TextFrameUnpack(QByteArray dataArr)
{
    TextFrame* data = new TextFrame;
    int flag = 4;

    data->length = getBigEndian_2Bit(dataArr.mid(flag, 2));
    flag += 2;
    data->localCode = dataArr[flag++];
    data->desCode = dataArr[flag++];
    data->deviceType = dataArr[flag++];
    data->captureState = dataArr[flag++];
    data->textDisplayState = dataArr[flag++];
    data->time = getDateTimeByArray(dataArr.mid(flag, 7));
    flag += 7;
    data->point.setX(getBigEndian_2Bit(dataArr.mid(flag, 2)));
    flag += 2;
    data->point.setY(getBigEndian_2Bit(dataArr.mid(flag, 2)));
    flag += 2;
    data->duration = dataArr[flag++];
    data->textLenght = dataArr[flag++];
    data->textColor = dataArr[flag++];
    data->text = dataArr.mid(flag, data->textLenght);
    flag += data->textLenght;

    return data;
}

Dialogue* TextAnalysis::TextFrameToDialogue(TextFrame* frame)
{
    Dialogue* data = new Dialogue;
    data->start = frame->time.toTime_t() - curTime.toTime_t();
    data->end = data->start + frame->duration;

    QString color;
    switch (frame->textColor)
    {
    case 0:  color = "00ffffff"; break;
    case 1:  color = "00ff0000"; break;
    case 2:  color = "00ff6100"; break;
    case 3:  color = "00ffff00"; break;
    case 4:  color = "00228b22"; break;
    case 5:  color = "0087ceeb"; break;
    case 6:  color = "001e90ff"; break;
    case 7:  color = "00a020f0"; break;
    default: color = "00ffffff"; break;
    }

    data->text = QString("{\\c&%1&}{\\pos(%2,%3)}%4")
            .arg(color)
            .arg(frame->point.x())
            .arg(frame->point.y())
            .arg(QString::fromLocal8Bit(frame->text));

    return data;
}

*/
void TextAnalysis::sendTextAnalysisMsg(int index, QPoint point, QString dev)
{
    TextFrame data;
    TextAnalysisInfo info;
    for(int i = 0; i < vecTextAnalysisInfo.size(); i++)
    {
        if(vecTextAnalysisInfo.at(i).index == index)
        {
            info = vecTextAnalysisInfo.at(i);
            break;
        }
    }
    data.captureState = 0xaa;
    data.textDisplayState = 0xb5;
    data.point = (point);
    data.duration = info.timeLength;

    data.textColor = info.color;
    info.text.replace("%", dev);
    data.text = info.text.toLocal8Bit();
    //data.text = info.text.toUtf8();
    data.textLenght = data.text.length();
    emit TextFramePacket(data);
}



