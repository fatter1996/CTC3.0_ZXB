#ifndef WORKSHEETS_H
#define WORKSHEETS_H

#include <QWidget>
#include "WorksheetTable.h"

namespace Ui {
class Worksheets;
}

class Worksheets : public QWidget
{
    Q_OBJECT

public:
    explicit Worksheets(QWidget *parent = nullptr);
    ~Worksheets();

    void setTitle(QString strTitle);
    void addSheet(QStringList strMune);

private:
    Ui::Worksheets *ui;
};

#endif // WORKSHEETS_H
