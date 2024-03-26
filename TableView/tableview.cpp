#include "tableview.h"
#include "ui_tableview.h"

#include <QStandardItemModel>
#include <QScrollBar>
#include <QTableWidgetItem>
#include <QDebug>
#include <QFont>
#include <QMouseEvent>
TableView::TableView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableView)
{
    ui->setupUi(this);
    installEventFilter(this);
    ui->tableWidget->installEventFilter(this);
    ui->tableWidget->verticalScrollBar()->setEnabled(true);
    ui->tableWidget->horizontalScrollBar()->setEnabled(true);
    ui->tableWidget->verticalHeader()->setMinimumWidth(32);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableWidget,&QTableWidget::clicked,[=](const QModelIndex &index){
        emit itemLBtnClicked(index);
    });
    connect(ui->tableWidget,&QTableWidget::doubleClicked,[=](const QModelIndex &index){
        emit itemRBtnDoubClicked(index);
    });

    connect(ui->tableWidget,&QTableWidget::customContextMenuRequested,[=](QPoint pt){
        QModelIndex index = ui->tableWidget->indexAt(pt);
        if(index.row() >= 0 && index.column() >= 0)
            emit itemRBtnClicked(index);
    });

}

TableView::~TableView()
{
    delete ui;
}

QTableWidgetItem* TableView::Item(int row, int col)
{
    return ui->tableWidget->item(row, col);
}

void TableView::IintTableColAndRow(int nrow,int ncol)
{
    ui->tableWidget->setColumnCount(ncol);
    ui->tableWidget->setRowCount(nrow);
}

void TableView::SetTableData(QStringList list,Qt::Alignment nAlign)
{
    int nColumnCount = mTableViewInfo.Vect_HeaderData.size();
    ui->tableWidget->setColumnCount(nColumnCount);
    int nlen = list.size();
    this->Clear();
    ui->tableWidget->setRowCount(nlen / nColumnCount);
    for(int i = 0; i < nlen / nColumnCount; i++)
    {
        QString str="";
        QTableWidgetItem mRowQStandardItem;
        mTableViewInfo.Vect_RowItem.append(mRowQStandardItem);
        for(int j=0;j<nColumnCount;j++)
        {
            if(nlen>((i*nColumnCount)+j))
                str=list[(i*nColumnCount)+j];
            else
                str="";
            QTableWidgetItem* item = new QTableWidgetItem(str);

            QString mStr="";
            QFontMetrics  fontMetrics(item->font());
            mStr=str;
            int fontSize = fontMetrics.horizontalAdvance(mStr);
            if (fontSize > mTableViewInfo.Vect_HeaderDataWidth[j])
            {
                item->setText(fontMetrics.elidedText(mStr, Qt::ElideRight, mTableViewInfo.Vect_HeaderDataWidth[j]+950));
                item->setToolTip(mStr);
            }
            else
            {
                item->setText(mStr);
                item->setToolTip("");
            }
            item->setTextAlignment(nAlign);
            ui->tableWidget->setItem(i,j,item);
        }
    }

}
void TableView::CreatTableView(bool b)
{
    int nColumnCount=mTableViewInfo.Vect_HeaderData.size();
    //表头列可拉伸
    if(b)
    {
       ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    }
    else
    {
        ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    }

    ui->tableWidget->setColumnCount(nColumnCount);
    QStringList headData;
    for(int i=0;i<nColumnCount;i++)
    {
        ui->tableWidget->setColumnWidth(i,mTableViewInfo.Vect_HeaderDataWidth[i]);
        headData << mTableViewInfo.Vect_HeaderData[i];
    }
    ui->tableWidget->setHorizontalHeaderLabels(headData);
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableWidget->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
}
void TableView::SetTableHeadHideH(bool b)
{
    ui->tableWidget->horizontalHeader()->setHidden(!b);

}
void TableView::SetTableHeadHideV(bool b)
{
    ui->tableWidget->verticalHeader()->setHidden(!b);
}
void TableView::SetTableStyle()
{

}

void TableView::SetTableHeadHeight(int ntype,int nheight)
{
    if(ntype == 0)
    {
        ui->tableWidget->horizontalHeader()->setDefaultSectionSize(nheight);
    }
    else if(ntype == 1)
    {
        ui->tableWidget->verticalHeader()->setMinimumSectionSize(nheight);
        ui->tableWidget->verticalHeader()->setMaximumSectionSize(nheight);
        ui->tableWidget->verticalHeader()->setDefaultSectionSize(nheight);
    }
}

void TableView::SetTableHeadWidth(int ntype, double diploid)
{
    if(ntype == 0)
    {
        nDiploid = diploid;
        for(int i=0; i<mTableViewInfo.Vect_HeaderDataWidth.size(); i++)
        {
            ui->tableWidget->setColumnWidth(i, (int)(mTableViewInfo.Vect_HeaderDataWidth[i]*nDiploid));
        }

    }
    else if(ntype == 1)
    {

    }
}

void TableView::SetTableHeadsetStyleSheet(int ntype,QString strStyle)
{
    if(ntype==0)
    {
        ui->tableWidget->setStyleSheet(strStyle);
    }
    else if(ntype == 1)
    {
        ui->tableWidget->horizontalHeader()->setStyleSheet(strStyle);
    }
    else if(ntype == 2)
    {
        ui->tableWidget->verticalHeader()->setStyleSheet(strStyle);
    }
}

void TableView::SetSectionText(QString text, int size)
{
    QAbstractButton*btn = ui->tableWidget->findChild<QAbstractButton*>();
    if (btn)
    {
        btn->setText(text);
        btn->installEventFilter(this);/*注册事件 CPersonMng::eventFilter*/
        QStyleOptionHeader opt;
        opt.text = btn->text();
        opt.textAlignment = Qt::AlignCenter;
        QSize s = btn->style()->sizeFromContents(QStyle::CT_HeaderSection, &opt, QSize(), btn).expandedTo(QApplication::globalStrut());
        if (s.isValid())
            ui->tableWidget->verticalHeader()->setMinimumWidth(size);
    }
}
void TableView::SetScrollBarV(bool v)
{
    ui->tableWidget->verticalScrollBar()->setDisabled(!v);
    ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tableWidget->verticalScrollBar()->setVisible(true);
}
void TableView::SetScrollBarH(bool h)
{
    ui->tableWidget->horizontalScrollBar()->setDisabled(h);
}

bool TableView::eventFilter(QObject* o, QEvent* e)
{
    if (e->type() == QEvent::Paint)
    {
        QAbstractButton* btn = qobject_cast<QAbstractButton*>(o);
        if (btn)
        {
            // paint by hand (borrowed from QTableCornerButton)

            QStyleOptionHeader opt;
            opt.init(btn);
            QStyle::State state = QStyle::State_None;
            if (btn->isEnabled())
                state |= QStyle::State_Enabled;
            if (btn->isActiveWindow())
                state |= QStyle::State_Active;
            if (btn->isDown())
                state |= QStyle::State_Sunken;
            opt.state = state;
            opt.rect = btn->rect();
            opt.text = btn->text();
            opt.textAlignment = Qt::AlignCenter;
            opt.position = QStyleOptionHeader::OnlyOneSection;
            QStylePainter painter(btn);
            painter.drawControl(QStyle::CE_Header, opt);
            return true;
        }
    }
    return QWidget::eventFilter(o, e);
}

void TableView::mouseReleaseEvent(QMouseEvent *event)
{

}
void TableView::SetTableDefaultAlignment(int n)
{
    if(n==0)
    {
        ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
        ui->tableWidget->verticalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    }
    else if(n==1)
    {
        ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
        ui->tableWidget->verticalHeader()->setDefaultAlignment(Qt::AlignLeft);

    }
}

void TableView::SetEditTriggers(QAbstractItemView::EditTriggers triggers)
{
    ui->tableWidget->setEditTriggers(triggers);
}

void TableView::SetItemCheck(int row,int col, Qt::CheckState acheckState)
{
    QTableWidgetItem * item = ui->tableWidget->item(row, col);

    if(item)
    {
        item->setFlags(item->flags()^Qt::ItemIsUserCheckable);
        item->setCheckState(acheckState);
    }
}

void TableView::SetRowItemsColor(int row, QColor bkcolor, QColor txtcolor, int startCol, int endCol)
{
    SetRowItemsBKColor(row, bkcolor, startCol, endCol);
    SetRowItemsTextColor(row, txtcolor, startCol, endCol);
}
void TableView::SetRowItemsBKColor(int row, QColor bkcolor, int startCol, int endCol)
{
    int endFlag = endCol;
    if(endCol == -1)
        endFlag = mTableViewInfo.Vect_HeaderData.size();
    for (int i = startCol; i < endFlag; i++)
    {
        QTableWidgetItem * item = ui->tableWidget->item(row, i);
        if(item) item->setBackground(bkcolor);
    }
}

void TableView::SetRowItemsTextColor(int row, QColor txtcolor, int startCol, int endCol)
{
    int endFlag = endCol;
    if(endCol == -1)
        endFlag = mTableViewInfo.Vect_HeaderData.size();
    for (int i = startCol; i < endFlag; i++)
    {
        QTableWidgetItem * item = ui->tableWidget->item(row, i);
        if(item) item->setData(Qt::TextColorRole, txtcolor);
    }
}

void TableView::SetRowItemsTextFont(int row, QFont font, int startCol, int endCol)
{
    int endFlag = endCol;
    if(endCol == -1)
        endFlag = mTableViewInfo.Vect_HeaderData.size();
    for (int i = startCol; i < endFlag; i++)
    {
        QTableWidgetItem * item = ui->tableWidget->item(row, i);
        if(item) item->setFont(font);
    }
}

void TableView::SetColItemsAlignment(int col, Qt::Alignment alignment, int startRow, int endRow)
{
    int endFlag = endRow;
    if(endRow == -1)
        endFlag = ui->tableWidget->rowCount();
    for (int i = startRow; i < endFlag; i++)
    {
        QTableWidgetItem * item = ui->tableWidget->item(i, col);
        if(item)
            item->setTextAlignment(alignment);
    }
}

void TableView::FontReSize(int nFontSize)
{
    QFont font = ui->tableWidget->font();
    font.setPointSize(nFontSize);
    ui->tableWidget->setFont(font);
}

void TableView::Clear()
{
    mTableViewInfo.Vect_RowItem.clear();
    ui->tableWidget->setRowCount(0);
}

void TableView::SelectRow(int row)
{
    ui->tableWidget->selectRow(row);
}
