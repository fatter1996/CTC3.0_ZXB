#ifndef WORKSHEETSDETAILSWND_H
#define WORKSHEETSDETAILSWND_H

#include <QWidget>

namespace Ui {
class WorksheetsDetailsWnd;
}

class WorksheetsDetailsWnd : public QWidget
{
    Q_OBJECT

public:
    explicit WorksheetsDetailsWnd(QWidget *parent = nullptr);
    ~WorksheetsDetailsWnd();

private:
    Ui::WorksheetsDetailsWnd *ui;
};

#endif // WORKSHEETSDETAILSWND_H
