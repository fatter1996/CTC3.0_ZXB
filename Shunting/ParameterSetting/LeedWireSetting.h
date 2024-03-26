#ifndef LEEDWIRESETTING_H
#define LEEDWIRESETTING_H

#include <QWidget>

namespace Ui {
class LeedWireSetting;
}

class LeedWireSetting : public QWidget
{
    Q_OBJECT

public:
    explicit LeedWireSetting(QWidget *parent = nullptr);
    ~LeedWireSetting();

private:
    Ui::LeedWireSetting *ui;
};

#endif // LEEDWIRESETTING_H
