#ifndef ROUTEPREVIEWWND_H
#define ROUTEPREVIEWWND_H

#include <QWidget>

namespace Ui {
class RoutePreviewWnd;
}

class RoutePreviewWnd : public QWidget
{
    Q_OBJECT

public:
    explicit RoutePreviewWnd(QWidget *parent = nullptr);
    ~RoutePreviewWnd();

private:
    Ui::RoutePreviewWnd *ui;
};

#endif // ROUTEPREVIEWWND_H
