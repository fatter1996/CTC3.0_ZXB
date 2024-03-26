#ifndef CANCELROUTEWND_H
#define CANCELROUTEWND_H

#include <QWidget>

namespace Ui {
class CancelRouteWnd;
}

class CancelRouteWnd : public QWidget
{
    Q_OBJECT

public:
    explicit CancelRouteWnd(QWidget *parent = nullptr);
    ~CancelRouteWnd();

private:
    Ui::CancelRouteWnd *ui;
};

#endif // CANCELROUTEWND_H
