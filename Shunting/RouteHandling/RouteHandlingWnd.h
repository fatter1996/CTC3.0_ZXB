#ifndef ROUTEHANDLINGWND_H
#define ROUTEHANDLINGWND_H

#include <QWidget>

namespace Ui {
class RouteHandlingWnd;
}

class RouteHandlingWnd : public QWidget
{
    Q_OBJECT

public:
    explicit RouteHandlingWnd(QWidget *parent = nullptr);
    ~RouteHandlingWnd();

private:
    Ui::RouteHandlingWnd *ui;
};

#endif // ROUTEHANDLINGWND_H
