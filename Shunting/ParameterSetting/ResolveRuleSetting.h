#ifndef RESOLVERULESETTING_H
#define RESOLVERULESETTING_H

#include <QWidget>

namespace Ui {
class ResolveRuleSetting;
}

class ResolveRuleSetting : public QWidget
{
    Q_OBJECT

public:
    explicit ResolveRuleSetting(QWidget *parent = nullptr);
    ~ResolveRuleSetting();

private:
    Ui::ResolveRuleSetting *ui;
};

#endif // RESOLVERULESETTING_H
