#ifndef PARAMETERSETWND_H
#define PARAMETERSETWND_H
#pragma execution_character_set("utf-8")
#include <QWidget>

namespace Ui {
class ParameterSetWnd;
}

class ParameterSetWnd : public QWidget
{
    Q_OBJECT

public:
    explicit ParameterSetWnd(QWidget *parent = nullptr);
    ~ParameterSetWnd();

    void addTabWidget(QWidget* _tebWid, QString wndName);
    void setDirectionSetting();
    void setPrioritySetting();
    void setRuleSetting();
private:
    Ui::ParameterSetWnd *ui;
};

#endif // PARAMETERSETWND_H
