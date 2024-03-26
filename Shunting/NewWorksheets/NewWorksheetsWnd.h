#ifndef WORKREQUISITIONWND_H
#define WORKREQUISITIONWND_H
#pragma execution_character_set("utf-8")
#include <QWidget>


namespace Ui {
class NewWorksheetsWnd;
}

class NewWorksheetsWnd : public QWidget
{
    Q_OBJECT
public:
    explicit NewWorksheetsWnd(QWidget *parent = nullptr);
    ~NewWorksheetsWnd();

private:
    Ui::NewWorksheetsWnd *ui;

};

#endif // WORKREQUISITION_H
