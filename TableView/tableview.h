#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QVector>
#include <QLabel>
#include <QPixmap>
#include <QTableView>
#include <QAbstractButton>
#include <QStylePainter>
#include <QAbstractItemView>
#include <QStyledItemDelegate>

namespace Ui {
class TableView;
}

class TableView : public QWidget
{
    Q_OBJECT

public:
    explicit TableView(QWidget *parent = nullptr);
    ~TableView();

    typedef  struct
    {
        QList<QTableWidgetItem*> mQStandardItem;
        void Init()
        {
           QList<QTableWidgetItem*>().swap(mQStandardItem);
        }
    }RowQStandardItem;//一行的信息

    typedef  struct
    {
        QVector<QString> Vect_HeaderData;
        QVector<int> Vect_HeaderDataWidth;
        QList<QTableWidgetItem*> Vect_StandardItem;
        QVector<QTableWidgetItem> Vect_RowItem;
        void Init()
        {
           QVector<QTableWidgetItem>().swap(Vect_RowItem);
           QVector<int>().swap(Vect_HeaderDataWidth);
        }
    }TableViewInfo;
    TableViewInfo mTableViewInfo;

    QTableWidgetItem* Item(int row, int col);
    void SetTableData(QStringList list,Qt::Alignment nAlign=Qt::AlignCenter);
    void CreatTableView(bool b=true);
    //设置表头隐藏/显示
    void SetTableHeadHideH(bool b);
    void SetTableHeadHideV(bool b);
    //设置表格样式
    void SetTableStyle();
    //设置表头高度
    //ntype：0：水平方向表头 1:垂直方向表头
    void SetTableHeadHeight(int ntype, int nheight);
    void SetTableHeadWidth(int ntype, double diploid);
    //设置表头样式
    //ntype：0：水平方向表头 1:垂直方向表头
    //strStyle:样式
    void SetTableHeadsetStyleSheet(int ntype,QString strStyle);
    void SetSectionText(QString text, int size);
    //设置表格行列
    void IintTableColAndRow(int nrow,int ncol);
    void SetScrollBarV(bool v);
    void SetScrollBarH(bool h);
    void InsterData(int ncol,int nrow, QString data);
    void SetEditTriggers(QAbstractItemView::EditTriggers triggers);
    void SetItemCheck(int row,int col, Qt::CheckState acheckState);

    void SetRowItemsColor(int row, QColor bkcolor, QColor txtcolor, int startCol = 0, int endCol = -1);
    void SetRowItemsBKColor(int row, QColor bkcolor, int startCol = 0, int endCol = -1);
    void SetRowItemsTextColor(int row, QColor txtcolor, int startCol = 0, int endCol = -1);
    void SetRowItemsTextFont(int row, QFont font, int startCol = 0, int endCol = -1);
    void SetColItemsAlignment(int col, Qt::Alignment alignment, int startRow = 0, int endRow = -1);

    void FontReSize(int nFontSize);
    void Clear();
    void SelectRow(int row);
    //设置文字显示 居中 靠左 靠右
    void SetTableDefaultAlignment(int n);

    bool eventFilter(QObject* o, QEvent* e) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void itemLBtnClicked(const QModelIndex &index);
    void itemRBtnClicked(const QModelIndex &index);
    void itemRBtnDoubClicked(const QModelIndex &index);
private:
    Ui::TableView *ui;
    double nDiploid = 1.0f;
};

#endif // TABLEVIEW_H
