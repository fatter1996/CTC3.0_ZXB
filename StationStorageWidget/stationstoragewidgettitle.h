#ifndef STATIONSTORAGEWIDGETTITLE_H
#define STATIONSTORAGEWIDGETTITLE_H

#include <QWidget>

namespace Ui {
class StationStorageWidgetTitle;
}

class StationStorageWidgetTitle : public QWidget
{
    Q_OBJECT

public:
    explicit StationStorageWidgetTitle(QWidget *parent = nullptr);
    ~StationStorageWidgetTitle();

private slots:
    void on_pushButton_clicked();

private:
    Ui::StationStorageWidgetTitle *ui;
};

#endif // STATIONSTORAGEWIDGETTITLE_H
