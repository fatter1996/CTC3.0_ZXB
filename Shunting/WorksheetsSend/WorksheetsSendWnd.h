#ifndef WORKSHEETSSENDWND_H
#define WORKSHEETSSENDWND_H

#include <QWidget>

namespace Ui {
class WorksheetsSendWnd;
}

class WorksheetsSendWnd : public QWidget
{
    Q_OBJECT

public:
    explicit WorksheetsSendWnd(QWidget *parent = nullptr);
    ~WorksheetsSendWnd();

private:
    Ui::WorksheetsSendWnd *ui;
};

#endif // WORKSHEETSSENDWND_H
