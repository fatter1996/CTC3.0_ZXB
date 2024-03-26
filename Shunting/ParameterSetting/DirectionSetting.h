#ifndef DIRECTIONSETTING_H
#define DIRECTIONSETTING_H

#include <QWidget>

namespace Ui {
class DirectionSetting;
}

class DirectionSetting : public QWidget
{
    Q_OBJECT

public:
    explicit DirectionSetting(QWidget *parent = nullptr);
    ~DirectionSetting();

private:
    Ui::DirectionSetting *ui;
};

#endif // DIRECTIONSETTING_H
