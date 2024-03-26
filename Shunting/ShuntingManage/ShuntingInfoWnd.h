#ifndef SHUNTINGINFOWND_H
#define SHUNTINGINFOWND_H
#pragma execution_character_set("utf-8")
#include <QWidget>
#include <QMenu>

namespace Ui {
class ShuntingInfoWnd;
}

class ShuntingInfoWnd : public QWidget
{
    Q_OBJECT

public:
    explicit ShuntingInfoWnd(QWidget *parent = nullptr);
    ~ShuntingInfoWnd();

    QMenu* addSheet(QStringList strMune);

private slots:
    void onTrainTypeChange(QAction *action);
    void onPrintChoice(QAction *action);

private:
    Ui::ShuntingInfoWnd *ui;

    QMenu* m_pTrainTypeMune = NULL;
    QMenu* m_pPrintMune = NULL;
};

#endif // SHUNTINGINFOWND_H
