#ifndef TRACKSETTING_H
#define TRACKSETTING_H

#include <QWidget>

namespace Ui {
class TrackSetting;
}

class TrackSetting : public QWidget
{
    Q_OBJECT

public:
    explicit TrackSetting(QWidget *parent = nullptr);
    ~TrackSetting();

private:
    Ui::TrackSetting *ui;
};

#endif // TRACKSETTING_H
