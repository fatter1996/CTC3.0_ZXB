#ifndef CBASEDATA_H
#define CBASEDATA_H
#include "GlobalHeaders/GlobalFuntion.h"
class CBaseData : public QObject
{
    Q_OBJECT
public:
    explicit CBaseData(QObject *parent = nullptr);

//封装成员变量
public:
    QPointF pCenter;            //设备中心点
    QPointF pCenterConst;       //缩放及偏移使用
    unsigned int m_nType;       //设备类型
    QString m_strName;          //设备名称
    unsigned int m_nSX;         //上下行咽喉 1:S 0:X
    unsigned int m_nCode;       //设备编号
    QRectF m_textRect;          //设备名称文本区域
    QRectF m_devRect;          //设备名称选中区域
    QRectF m_textRectConst;     //设备名称文本区域
    unsigned int m_nStationID;           //所属站ID
    unsigned int m_nBelongToTCC;         //所属TCC ID
    unsigned int m_nBelongToRBC;         //所属RBC ID
    bool isDisPlayName;         //是否显示名称
    bool m_bToolTip;            //是否显示鼠标提示信息
    QString m_strToolTip;       //鼠标提示信息

    int WndType = 0; //窗口类型 1-进路预览,2-批量操作
    int m_nState;                              // 此组件对象的状态
    int m_nOldState;                           // 此组件对象的上一次状态
    int m_nOldState2;                          // 此组件对象的上上一次状态
    bool m_RangeVisible; //设置设备范围显示（或高亮显示）
    bool m_RangeVisible_XZ; //设备被选中后的显示
    bool gIsConnectInterlock;
    bool bFCZKState;//非常站控状态

    bool m_RangeVisible_XHD; //设置设备范围显示（或高亮显示）
    bool m_RangeVisible_LCAN; //设置设备范围显示（或高亮显示）
    bool m_RangeVisible_TGAN; //设置设备范围显示（或高亮显示）
    bool m_RangeVisible_DCAN; //设置设备范围显示（或高亮显示）
    bool m_RangeVisible_Name; //设置设备范围显示（或高亮显示）
    bool m_RangeVisible_XHD_DDMD; //设置设备范围显示（或高亮显示）//点灯灭灯按下
    bool m_RangeVisible_XHD_YDAN; //设置设备范围显示（或高亮显示）//点灯灭灯按下

    bool m_RangeVisible_ZDBS_ZFZ; //设置设备范围显示（或高亮显示）//点灯灭灯按下
    bool m_RangeVisible_ZDBS_JCFZ; //设置设备范围显示（或高亮显示）//点灯灭灯按下
    bool m_RangeVisible_ZDBS_FCFZ; //设置设备范围显示（或高亮显示）//点灯灭灯按下
    bool m_RangeVisible_ZDBS_GF; //设置设备范围显示（或高亮显示）//点灯灭灯按下
    bool m_RangeVisible_BZDBS_BSQH; //设置设备范围显示（或高亮显示）//点灯灭灯按下
    bool m_RangeVisible_BZDBS_BS; //设置设备范围显示（或高亮显示）//点灯灭灯按下
    bool m_RangeVisible_BZDBS_SG; //设置设备范围显示（或高亮显示）//点灯灭灯按下
    bool m_RangeVisible_BZDBS_FY; //设置设备范围显示（或高亮显示）//点灯灭灯按下
    bool m_RangeVisible_JZ_JZSY; //设置设备范围显示（或高亮显示）//点灯灭灯按下
    bool m_RangeVisible_JZ_JZTY; //设置设备范围显示（或高亮显示）//点灯灭灯按下
    bool m_RangeVisible_JZ_JZFL; //设置设备范围显示（或高亮显示）//点灯灭灯按下
    bool m_RangeVisible_JZ_BSQH; //设置设备范围显示（或高亮显示）//点灯灭灯按下
public:
    void setState(int nState) ;// * 设置此对象组件的状态  * @param nState 该对象组件的状态参数
    bool getState(int nState) ;// 取得此对象组件是否具有所给状态   * @param nState 所给状态参数
    bool getOldState(int nState) ;// 取得对象上一次状态
    int  getOldState() ;// 取得对象上一次状态
    bool getOldState2(int nState) ;// 取得对象上上一次状态
    int  getState() ;// 取得此对象组件的状态   * @return 返回值即为该组件对象的状态

//封装成员变量接口函数
public:
    void setCenterPt(QPointF pt);  //设置设备中心点
    QPointF getCenterPt();         //获取设备中心点
    void setType(unsigned int nType);   //设置设备类型
    unsigned int getType();             //获取设备类型
    void setName(QString name);         //设置设备名称
    QString getName();                  //获取设备名称
    void setSX(unsigned int nSX);       //设置上下行咽喉
    unsigned int getSX();               //获取上下行咽喉
    void setCode(unsigned int code);    //设置设备编号
    unsigned int getCode();             //获取设备编号
    void setTextRect(QRectF rect);      //设置设备名称文本区域
    QRectF getTextRect();               //获取设备名称文本区域
    void setStationID(unsigned int id); //设置所属站ID
    unsigned int getStationID();        //获取所属站ID
    void setBelongToTCC(unsigned int id);    //设置所属TCC ID
    unsigned int getBelongToTCC();           //获取所属TCC ID
    void setBelongToRBC(unsigned int id);    //设置所属RBC ID
    unsigned int getBelongToRBC();           //获取所属RBC ID
    void setDisplayName(bool nFlag);    //设置是否显示名称
    bool getDisplayName();              //获取是否显示名称
    void setToolTipFlag(bool flag);     //设置是否显示鼠标提示信息
    bool getToolTipFlag();              //获取是否显示鼠标提示信息
    void setToolTipStr(QString str);    //设置鼠标提示信息
    QString getToolTipStr();            //获取鼠标提示信息

//封装虚函数用于实现多态
public:
    virtual void Draw_ToolTip(QPainter *painter, double nDiploid);
    virtual void Draw(QPainter *painter, long nElapsed, double nDiploid,QPoint offset,int type);
    virtual void DrawM(QPainter *painter, long nElapsed, double nDiploid,QPoint offset,int type, int flag = 0);
    virtual void setDevStateToSafe();
    virtual unsigned int getDevType();
    virtual int moveCursor(QPoint p);
    virtual void setVollover(QPoint pt_Base);

    void draw_Pixmap(QPainter *painter,QString picName,int x,int y,int width,int heigh,int nDiploid);  //绘制图片
};

#endif // CBASEDATA_H
