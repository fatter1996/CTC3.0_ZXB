#ifndef GDDATA_H
#define GDDATA_H

#include "GlobalHeaders/GlobalFuntion.h"
#include "stationstorage.h"


class GDData: public StationStorage
{
public:
    GDData();
    QString TrackName="";//股道名称
    QString TrackTrainName;//股道中车辆名称
    bool bSX = true;//上下行 true：上行 false：下行
    QVector<TableShowData>mVectTableShowData;

    void Init();
};

#endif // GDDATA_H
