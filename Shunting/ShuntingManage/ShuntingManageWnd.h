#ifndef SHUNTINGMANAGEWND_H
#define SHUNTINGMANAGEWND_H

#include <QWidget>

#include "ShuntingInfoWnd.h"

namespace Ui {
class ShuntingManageWnd;
}

class ShuntingManageWnd : public QWidget
{
    Q_OBJECT

public:
    explicit ShuntingManageWnd(QWidget *parent = nullptr);
    ~ShuntingManageWnd();

    void addNewTabWidget(QString wndName);

    void moveEvent(QMoveEvent *event);

private slots:
    void on_currentRBtn_toggled(bool checked);

    void on_historyRBtn_toggled(bool checked);

    void on_showSetOutRBtn_toggled(bool checked);

    void on_pagingBtn_clicked();



    void on_columnarBtn_clicked();

private:
    Ui::ShuntingManageWnd *ui;

    QVector<ShuntingInfoWnd*> m_tabWidgetList;

    bool isPaging = false;
};

#endif // SHUNTINGMANAGEWND_H
