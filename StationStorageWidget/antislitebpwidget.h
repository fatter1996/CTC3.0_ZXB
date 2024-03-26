#ifndef ANTISLITEBPWIDGET_H
#define ANTISLITEBPWIDGET_H

#include <QWidget>

namespace Ui {
class AntisliTebpWidget;
}

class AntisliTebpWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AntisliTebpWidget(QWidget *parent = nullptr);
    ~AntisliTebpWidget();

private:
    Ui::AntisliTebpWidget *ui;
};

#endif // ANTISLITEBPWIDGET_H
