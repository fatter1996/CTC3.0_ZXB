#include "jz.h"

CJZ::CJZ()
{
    m_nGLXH=0xffff;
    isNX=0;
    ArrowPt=QPoint(0,0);
    ArrowPtConst=QPoint(0,0);
    m_nAnNum=0;
    p_JZSYAN=QPoint(0,0);
    p_JZSYANConst=QPoint(0,0);
    p_JZTYAN=QPoint(0,0);
    p_JZTYANConst=QPoint(0,0);
    p_JZFLAN=QPoint(0,0);
    p_JZFLANConst=QPoint(0,0);
    p_BSQHAN=QPoint(0,0);
    p_BSQHANConst=QPoint(0,0);
    p_MN=QPoint(0,0);
    p_MNConst=QPoint(0,0);
    rect_JZSY=QRect(0,0,0,0);
    rect_JZTY=QRect(0,0,0,0);
    rect_JZFL=QRect(0,0,0,0);
    rect_BSQH=QRect(0,0,0,0);
    isDown_JZSY=false;
    isDown_JZTY=false;
    isDown_JZFL=false;
    isDown_BSQH=false;
//    m_nDownTime_FCFZ=0xFF;
//    m_nDownTime_JCFZ=0xFF;
    p_BSD_JZSY=QPoint(0,0); //允许发车灯
    p_BSD_JZSYConst=QPoint(0,0); //允许发车灯
    p_BSD_JZTY=QPoint(0,0);  //辅助表示灯
    p_BSD_JZTYConst=QPoint(0,0);  //辅助表示灯
    p_BSD_JZFL=QPoint(0,0);  //闪光表示灯
    p_BSD_JZFLConst=QPoint(0,0);  //闪光表示灯
    p_BSD_JZBJ=QPoint(0,0);  //区间表示灯
    p_BSD_JZBJConst=QPoint(0,0);  //区间表示灯
    p_BSD_QJ=QPoint(0,0);  //区间表示灯
    p_BSD_QJConst=QPoint(0,0);  //区间表示灯
    rect_FK=QRect(0,0,0,0);
    rect_FKConst=QRect(0,0,0,0);
    isBSDLight_JZSY=false;
    isBSDLight_JZTY=false;
    isBSDLight_JZFL=false;
    isBSDLight_JZBJ=false;
    isBSDLight_QJ=false;
    m_nArrowStateJC=Arrow_Black;
    m_nArrowStateFC=Arrow_Black;

    JZSYAN_Name = "";
    JZTYAN_Name = "";
    JZFLAN_Name = "";
    BSQHAN_Name = "";
    JZSYD_Name = "";
    JZTYD_Name = "";
    JZFLD_Name = "";
    JZBJD_Name = "";
    QJD_Name = "";
    ArrowLeftName = "";
    ArrowRightName = "";

    haveJZSYAN = true;
    haveJZTYAN = true;
    haveJZFLAN = true;
    haveBSQHAN = true;
    haveJZSYD = true;
    haveJZTYD = true;
    haveJZFLD = true;
    haveJZBJD = true;
    haveQJ = true;

    JZSYAN_QF = 0;
    JZTYAN_QF = 0;
    JZFLAN_QF = 0;
    BSQHAN_QF = 0;

    haveFK = false;
    ptFK=QPoint(0,0);
    FK_length = 0;
    FK_breath = 0;

    haveWord = false;
    ptWord = QPoint(0,0);
    WordName = "";

    m_RangeVisible = false;
    ButtonForm = 1;

    m_RangeVisible_JZ_JZSY = false; //设置设备范围显示（或高亮显示）//点灯灭灯按下
    m_RangeVisible_JZ_JZTY = false; //设置设备范围显示（或高亮显示）//点灯灭灯按下
    m_RangeVisible_JZ_JZFL = false; //设置设备范围显示（或高亮显示）//点灯灭灯按下
    m_RangeVisible_JZ_BSQH = false; //设置设备范围显示（或高亮显示）//点灯灭灯按下

    m_nDownTime_JZFL_1 = 0;   //当按钮按下为1时持续时间
    m_nDownTime_JZSY_1 = 0;
    m_nDownTime_JZTY_1 = 0;
    m_nDownTime_JZBSQH_1 = 0;
}
void CJZ::Draw_ToolTip(QPainter *painter, double nDiploid)
{
    QFont font;
    QPoint pt;
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);
    //绘制鼠标进入文字提示信息
    if((true == getToolTipFlag()) && (""!=getToolTipStr()))
    {
        pt.setX(pCenter.x()+5*nDiploid);
        pt.setY(pCenter.y()-10*nDiploid);
        font.setFamily("宋体");
        font.setPointSize(9 * nDiploid);//字号
        font.setItalic(false);//斜体

        QFontMetrics fm(font);
        QRect rec = fm.boundingRect(getToolTipStr());

        painter->setPen(QPen(QColor(38, 38, 38), 1));
        painter->setBrush(QColor(252, 245, 221));
        painter->drawRect(pt.x(), pt.y(), rec.width()+10*nDiploid, rec.height()+4*nDiploid);

        painter->setFont(font);//设置字体
        painter->setPen(QColor(38, 38, 38));//设置画笔颜色
        //绘制文本
        painter->drawText(pt.x()+5*nDiploid, pt.y()+12* nDiploid, getToolTipStr());
    }
}
int CJZ::moveCursor(QPoint p)
{
    if (rect_JZSY.contains(p))
    {
        return 1;
    }
    else if(rect_JZTY.contains(p))
    {
        return 2;
    }
    else if(rect_JZFL.contains(p))
    {
        return 3;
    }
    else if(getAnNum() == 4)
    {
        if (rect_BSQH.contains(p))
        {
            return 4;
        }
    }
    return 0;
}
void CJZ::Draw(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type)
{
    if(type==0x55)
    {
        Draw_Th_kb(painter,nElapsed, nDiploid,offset);
    }
}
void CJZ::Draw_Th_kb(QPainter *painter,long nElapsed, double nDiploid,QPoint offset)
{
    QFont font;
    ArrowPt.setX(ArrowPtConst.x() * nDiploid+offset.x()*nDiploid);
    ArrowPt.setY(ArrowPtConst.y() * nDiploid+offset.y()*nDiploid);

    if(ButtonForm == 1)
    {
        rect_JZSY.setRect(p_JZSYANConst.x() * nDiploid + offset.x() * nDiploid,p_JZSYANConst.y() * nDiploid + offset.y() * nDiploid,10*nDiploid,10*nDiploid);
        rect_JZTY.setRect(p_JZTYANConst.x() * nDiploid + offset.x() * nDiploid,p_JZTYANConst.y() * nDiploid + offset.y() * nDiploid,10*nDiploid,10*nDiploid);
        rect_JZFL.setRect(p_JZFLANConst.x() * nDiploid + offset.x() * nDiploid,p_JZFLANConst.y() * nDiploid + offset.y() * nDiploid,10*nDiploid,10*nDiploid);
        rect_BSQH.setRect(p_BSQHANConst.x() * nDiploid + offset.x() * nDiploid,p_BSQHANConst.y() * nDiploid + offset.y() * nDiploid,10*nDiploid,10*nDiploid);
    }
    else
    {
        rect_JZSY.setRect(p_JZSYANConst.x() * nDiploid + offset.x() * nDiploid,p_JZSYANConst.y() * nDiploid + offset.y() * nDiploid,14*nDiploid,14*nDiploid);
        rect_JZTY.setRect(p_JZTYANConst.x() * nDiploid + offset.x() * nDiploid,p_JZTYANConst.y() * nDiploid + offset.y() * nDiploid,14*nDiploid,14*nDiploid);
        rect_JZFL.setRect(p_JZFLANConst.x() * nDiploid + offset.x() * nDiploid,p_JZFLANConst.y() * nDiploid + offset.y() * nDiploid,14*nDiploid,14*nDiploid);
        rect_BSQH.setRect(p_BSQHANConst.x() * nDiploid + offset.x() * nDiploid,p_BSQHANConst.y() * nDiploid + offset.y() * nDiploid,14*nDiploid,14*nDiploid);
    }

    p_BSD_JZSY.setX(p_BSD_JZSYConst.x() * nDiploid+offset.x()*nDiploid);
    p_BSD_JZSY.setY(p_BSD_JZSYConst.y() * nDiploid+offset.y()*nDiploid);
    p_BSD_JZTY.setX(p_BSD_JZTYConst.x() * nDiploid+offset.x()*nDiploid);
    p_BSD_JZTY.setY(p_BSD_JZTYConst.y() * nDiploid+offset.y()*nDiploid);
    p_BSD_JZFL.setX(p_BSD_JZFLConst.x() * nDiploid+offset.x()*nDiploid);
    p_BSD_JZFL.setY(p_BSD_JZFLConst.y() * nDiploid+offset.y()*nDiploid);
    p_BSD_JZBJ.setX(p_BSD_JZBJConst.x() * nDiploid+offset.x()*nDiploid);
    p_BSD_JZBJ.setY(p_BSD_JZBJConst.y() * nDiploid+offset.y()*nDiploid);
    p_BSD_QJ.setX(p_BSD_QJConst.x() * nDiploid+offset.x()*nDiploid);
    p_BSD_QJ.setY(p_BSD_QJConst.y() * nDiploid+offset.y()*nDiploid);
//    rect_FK.setRect(rect_FKConst.topLeft().x() * nDiploid + offset.x() * nDiploid,rect_FKConst.topLeft().y() * nDiploid + offset.y() * nDiploid,rect_FKConst.width()*nDiploid,rect_FKConst.height()*nDiploid);
    rect_FK.setRect(ptFK.x() * nDiploid + offset.x() * nDiploid,ptFK.y() * nDiploid + offset.y() * nDiploid,FK_length*nDiploid,FK_breath*nDiploid);
    int WordNameLenth;
    WordNameLenth = WordName.length();
    rect_Word.setRect(ptWord.x() * nDiploid + offset.x() * nDiploid,ptWord.y() * nDiploid + offset.y() * nDiploid,WordNameLenth*12*nDiploid,20*nDiploid);
    //rect_Word.setRect(ptWord.x() * nDiploid + offset.x() * nDiploid,ptWord.y() * nDiploid + offset.y() * nDiploid,60*nDiploid,20*nDiploid);
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);

    font.setFamily("宋体");
    font.setPointSize(10 * nDiploid);//字号
    font.setItalic(false);//斜体
    painter->setFont(font);//设置字体
    painter->setPen(Qt::white);//设置画笔颜色

    QString strNameAN[4]={"总辅助","发车辅助","接车辅助","改方按钮"};
    bool isDownAN[4]={false,false,false,false};
    isDownAN[0]=getIsDown_JZSY();
    isDownAN[1]=getIsDown_JZTY();
    isDownAN[2]=getIsDown_JZFL();
    isDownAN[3]=getIsDown_BSQH();
    QRect rectAN[4]={rect_JZSY,rect_JZTY,rect_JZFL,rect_BSQH};

    QString strNameBSD[4]={"允许发车","辅助","闪光","区间"};
    bool isLight[4]={false,false,false,false};
    isLight[0]=getIsBSDLight_JZSY();
    isLight[1]=getIsBSDLight_JZTY();
    isLight[2]=getIsBSDLight_JZFL();
    isLight[3]=getIsBSDLight_JZBJ();
    QPoint bsdPt[4]={p_BSD_JZSY,p_BSD_JZTY,p_BSD_JZFL,p_BSD_JZBJ};

//    DrawZDBSArrow(painter,nElapsed,nDiploid,offset);//绘制箭头
//    //if(VISIBLE_JSQ)
//    {//绘制自动闭塞铅封
//        QPoint ArrowQF;
//        ArrowQF.setX(ArrowPt.x()+56*nDiploid);
//        ArrowQF.setY(ArrowPt.y()+28*nDiploid);
//        DrawQFRect(painter,ArrowQF, ZDBS_QF, nElapsed,nDiploid,offset);
//    }
    if(haveJZSYAN)
    {
        if(ButtonForm == 1)
        {
            Draw_Rect_Button(painter,//画笔
                             rect_JZSY,//按钮绘制区域//需要修改按钮区域大小为10
                             bFCZKState,//非常站控状态
                             getIsDown_JZSY(),//按钮是否按下
                             false,//封锁状态
                             false,//终端闪烁状态
                             0,//倒计时时间
                             THIN_GRAY,//按钮颜色
                             JZSYAN_Name, //按钮文字显示
                             m_RangeVisible_JZ_JZSY,//按钮是否高亮
                             nElapsed,//闪烁读秒
                             nDiploid,//放大倍数
                             offset);//绘制KSK方形按钮
        }
        else
        {
            Draw_circle_Button(painter,//画笔
                               rect_JZSY,//按钮绘制区域//需要修改按钮区域大小为14
                               bFCZKState,//非常站控状态
                               getIsDown_JZSY(),//按钮是否按下
                               false,//封锁状态
                               false,//终端闪烁状态
                               0,//倒计时时间
                               JZSYAN_Name, //按钮文字显示
                               m_RangeVisible_JZ_JZSY,//按钮是否高亮
                               nElapsed, //闪烁读秒
                               nDiploid, //放大倍数
                               offset);//绘制圆形按钮
        }
        //if(VISIBLE_JSQ)
        if(bShowQFJS)
        {//绘制总辅助铅封
            if(bVisleJSQ)
            {
                QPoint ZFZQF;
                ZFZQF.setX(rect_JZSY.x()-8*nDiploid);
                ZFZQF.setY(rect_JZSY.y()-20*nDiploid);
                DrawQFRect(painter,ZFZQF, JZSYAN_QF, nElapsed,nDiploid,offset);
            }
        }
    }
    if(haveJZTYAN)
    {
        if(ButtonForm == 1)
        {
            Draw_Rect_Button(painter,//画笔
                             rect_JZTY,//按钮绘制区域//需要修改按钮区域大小为10
                             bFCZKState,//非常站控状态
                             getIsDown_JZTY(),//按钮是否按下
                             false,//封锁状态
                             false,//终端闪烁状态
                             0,//倒计时时间
                             THIN_GRAY,//按钮颜色
                             JZTYAN_Name, //按钮文字显示
                             m_RangeVisible_JZ_JZTY,//按钮是否高亮
                             nElapsed,//闪烁读秒
                             nDiploid,//放大倍数
                             offset);//绘制KSK方形按钮
        }
        else
        {
            Draw_circle_Button(painter,//画笔
                               rect_JZTY,//按钮绘制区域//需要修改按钮区域大小为14
                               bFCZKState,//非常站控状态
                               getIsDown_JZTY(),//按钮是否按下
                               false,//封锁状态
                               false,//终端闪烁状态
                               0,//倒计时时间
                               JZTYAN_Name, //按钮文字显示
                               m_RangeVisible_JZ_JZTY,//按钮是否高亮
                               nElapsed, //闪烁读秒
                               nDiploid, //放大倍数
                               offset);//绘制圆形按钮
        }
        //if(VISIBLE_JSQ)
        if(bShowQFJS)
        {//绘制发车辅助铅封
            if(bVisleJSQ)
            {
                QPoint FCFZQF;
                FCFZQF.setX(rect_JZTY.x()-8*nDiploid);
                FCFZQF.setY(rect_JZTY.y()-20*nDiploid);
                DrawQFRect(painter,FCFZQF, JZTYAN_QF, nElapsed,nDiploid,offset);
            }
        }
    }
    if(haveJZFLAN)
    {
        if(ButtonForm == 1)
        {
            Draw_Rect_Button(painter,//画笔
                             rect_JZFL,//按钮绘制区域//需要修改按钮区域大小为10
                             bFCZKState,//非常站控状态
                             getIsDown_JZFL(),//按钮是否按下
                             false,//封锁状态
                             false,//终端闪烁状态
                             0,//倒计时时间
                             THIN_GRAY,//按钮颜色
                             JZFLAN_Name, //按钮文字显示
                             m_RangeVisible_JZ_JZFL,//按钮是否高亮
                             nElapsed,//闪烁读秒
                             nDiploid,//放大倍数
                             offset);//绘制KSK方形按钮
        }
        else
        {
            Draw_circle_Button(painter,//画笔
                               rect_JZFL,//按钮绘制区域//需要修改按钮区域大小为14
                               bFCZKState,//非常站控状态
                               getIsDown_JZFL(),//按钮是否按下
                               false,//封锁状态
                               false,//终端闪烁状态
                               0,//倒计时时间
                               JZFLAN_Name, //按钮文字显示
                               m_RangeVisible_JZ_JZFL,//按钮是否高亮
                               nElapsed, //闪烁读秒
                               nDiploid, //放大倍数
                               offset);//绘制圆形按钮
        }
        //if(VISIBLE_JSQ)
        if(bShowQFJS)
        {//绘制接车辅助铅封
            if(bVisleJSQ)
            {
                QPoint JCFZQF;
                JCFZQF.setX(rect_JZFL.x()-8*nDiploid);
                JCFZQF.setY(rect_JZFL.y()-20*nDiploid);
                DrawQFRect(painter,JCFZQF, JZFLAN_QF, nElapsed,nDiploid,offset);
            }
        }
    }
    if(haveBSQHAN)
    {
        if(ButtonForm == 1)
        {
            Draw_Rect_Button(painter,//画笔
                             rect_BSQH,//按钮绘制区域//需要修改按钮区域大小为10
                             bFCZKState,//非常站控状态
                             getIsDown_BSQH(),//按钮是否按下
                             false,//封锁状态
                             false,//终端闪烁状态
                             0,//倒计时时间
                             THIN_GRAY,//按钮颜色
                             BSQHAN_Name, //按钮文字显示
                             m_RangeVisible_JZ_BSQH,//按钮是否高亮
                             nElapsed,//闪烁读秒
                             nDiploid,//放大倍数
                             offset);//绘制KSK方形按钮
        }
        else
        {
            Draw_circle_Button(painter,//画笔
                               rect_BSQH,//按钮绘制区域//需要修改按钮区域大小为14
                               bFCZKState,//非常站控状态
                               getIsDown_BSQH(),//按钮是否按下
                               false,//封锁状态
                               false,//终端闪烁状态
                               0,//倒计时时间
                               BSQHAN_Name, //按钮文字显示
                               m_RangeVisible_JZ_BSQH,//按钮是否高亮
                               nElapsed, //闪烁读秒
                               nDiploid, //放大倍数
                               offset);//绘制圆形按钮
        }
        //if(VISIBLE_JSQ)
        if(bShowQFJS)
        {//绘制改方铅封
            if(bVisleJSQ)
            {
                QPoint GFQF;
                GFQF.setX(rect_BSQH.x()-8*nDiploid);
                GFQF.setY(rect_BSQH.y()-20*nDiploid);
                DrawQFRect(painter,GFQF, BSQHAN_QF, nElapsed,nDiploid,offset);
            }
        }
    }

    if(haveJZSYD)
    {
        Draw_Rect_Light(painter,//画笔
                        getBSD_JZSYPt(),//指示灯中心坐标
                        JZSYD_Name, //文字显示
                        getIsBSDLight_JZSY(),
                        3,
                        nElapsed,//闪烁读秒
                        nDiploid,//放大倍数
                        offset);//绘制指示灯
    }
    if(haveJZTYD)
    {
        Draw_Rect_Light(painter,//画笔
                        getBSD_JZTYPt(),//指示灯中心坐标
                        JZTYD_Name, //文字显示
                        getIsBSDLight_JZTY(),
                        1,
                        nElapsed,//闪烁读秒
                        nDiploid,//放大倍数
                        offset);//绘制指示灯
    }
    if(haveJZFLD)
    {
        Draw_Rect_Light(painter,//画笔
                        getBSD_JZFLPt(),//指示灯中心坐标
                        JZFLD_Name, //文字显示
                        getIsBSDLight_JZFL(),
                        4,
                        nElapsed,//闪烁读秒
                        nDiploid,//放大倍数
                        offset);//绘制指示灯
    }
    if(haveJZBJD)
    {
        Draw_Rect_Light(painter,//画笔
                        getBSD_JZBJPt(),//指示灯中心坐标
                        JZBJD_Name, //文字显示
                        getIsBSDLight_JZBJ(),
                        1,
                        nElapsed,//闪烁读秒
                        nDiploid,//放大倍数
                        offset);//绘制指示灯
    }
    if(haveQJ)
    {
        Draw_Rect_Light(painter,//画笔
                        getBSD_QJPt(),//指示灯中心坐标
                        QJD_Name, //文字显示
                        getIsBSDLight_QJ(),
                        1,
                        nElapsed,//闪烁读秒
                        nDiploid,//放大倍数
                        offset);//绘制指示灯
    }

    //绘制方框
    if(haveFK)
    {
        painter->setPen(QPen(Qt::white, 1));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect_FK);
    }
    //绘制文字
    if(haveWord)
    {
        QFontMetrics fm(font);
        QRect rec = fm.boundingRect(getName());

        painter->setPen(QPen(Qt::black, 1));
        painter->setBrush(Qt::black);
        painter->drawRect(rect_Word);

        painter->setPen(Qt::white);//设置画笔颜色
        painter->drawText(rect_Word,Qt::AlignHCenter|Qt::AlignVCenter,WordName);
    }
}
void CJZ::setVollover(QPoint pt_Base)
{

}
unsigned int CJZ::getDevType()
{
    return Dev_JZ;
}

void CJZ::setGLXH(int x)
{
    m_nGLXH=x;
}
int CJZ::getGLXH()
{
    return m_nGLXH;
}
void CJZ::addVectJJQD(int x)
{
    vect_JJQD.append(x);
}
void CJZ::setVectJJQD(QVector<int> x)
{
    vect_JJQD=x;
}
QVector<int> CJZ::getVectJJQD()
{
    return vect_JJQD;
}
void CJZ::setIsNX(bool flag)
{
    isNX=flag;
}
bool CJZ::getIsNX()
{
    return isNX;
}
void CJZ::setArrowPt(QPoint pt)
{
    ArrowPt=pt;
    ArrowPtConst=pt;
}
QPoint CJZ::getArrowPt()
{
    return ArrowPtConst;
}
void CJZ::setAnNum(int x)
{
    m_nAnNum=x;
}
int CJZ::getAnNum()
{
    return m_nAnNum;
}
void CJZ::setJZSYANPt(QPoint pt)
{
    p_JZSYAN=pt;
    p_JZSYANConst=pt;
}
QPoint CJZ::getJZSYANPt()
{
    return p_JZSYANConst;
}
void CJZ::setJZTYANPt(QPoint pt)
{
    p_JZTYAN=pt;
    p_JZTYANConst=pt;
}
QPoint CJZ::getJZTYANPt()
{
    return p_JZTYANConst;
}
void CJZ::setJZFLANPt(QPoint pt)
{
    p_JZFLAN=pt;
    p_JZFLANConst=pt;
}
QPoint CJZ::getJZFLANPt()
{
    return p_JZFLANConst;
}
void CJZ::setBSQHANPt(QPoint pt)
{
    p_BSQHAN=pt;
    p_BSQHANConst=pt;
}
QPoint CJZ::getBSQHANPt()
{
    return p_BSQHANConst;
}
void CJZ::setMNPt(QPoint pt)
{
    p_MN=pt;
    p_MNConst=pt;
}
QPoint CJZ::getMNPt()
{
    return p_MNConst;
}
void CJZ::setIsDown_JZSY(int flag)
{
    isDown_JZSY=flag;
}
int CJZ::getIsDown_JZSY()
{
    return isDown_JZSY;
}
void CJZ::setIsDown_JZTY(int flag)
{
    isDown_JZTY=flag;
}
int CJZ::getIsDown_JZTY()
{
    return isDown_JZTY;
}
void CJZ::setIsDown_JZFL(int flag)
{
    isDown_JZFL=flag;
}
int CJZ::getIsDown_JZFL()
{
    return isDown_JZFL;
}
void CJZ::setIsDown_BSQH(int flag)
{
    isDown_BSQH=flag;
}
int CJZ::getIsDown_BSQH()
{
    return isDown_BSQH;
}
//void CJZ::setDownTime_FCFZ(int x)
//{
//    m_nDownTime_FCFZ=x;
//}
//int CJZ::getDownTime_FCFZ()
//{
//    return m_nDownTime_FCFZ;
//}
//void CJZ::setDownTime_JCFZ(int x)
//{
//    m_nDownTime_JCFZ=x;
//}
//int CJZ::getDownTime_JCFZ()
//{
//    return m_nDownTime_JCFZ;
//}
void CJZ::setBSD_JZSYPt(QPoint pt)
{
    p_BSD_JZSY=pt;
    p_BSD_JZSYConst=pt;
}
QPoint CJZ::getBSD_JZSYPt()
{
    return p_BSD_JZSY;
}
void CJZ::setBSD_JZTYPt(QPoint pt)
{
    p_BSD_JZTY=pt;
    p_BSD_JZTYConst=pt;
}
QPoint CJZ::getBSD_JZTYPt()
{
    return p_BSD_JZTY;
}
void CJZ::setBSD_JZFLPt(QPoint pt)
{
    p_BSD_JZFL=pt;
    p_BSD_JZFLConst=pt;
}
QPoint CJZ::getBSD_JZFLPt()
{
    return p_BSD_JZFL;
}
void CJZ::setBSD_JZBJPt(QPoint pt)
{
    p_BSD_JZBJ=pt;
    p_BSD_JZBJConst=pt;
}
QPoint CJZ::getBSD_JZBJPt()
{
    return p_BSD_JZBJ;
}
void CJZ::setBSD_QJPt(QPoint pt)
{
    p_BSD_QJ=pt;
    p_BSD_QJConst=pt;
}
QPoint CJZ::getBSD_QJPt()
{
    return p_BSD_QJ;
}
void CJZ::setRect_FK(QRect rect)
{
    rect_FK=rect;
    rect_FKConst=rect;
}
QRect CJZ::getRect_FK()
{
    return rect_FKConst;
}
void CJZ::setIsBSDLight_JZSY(bool flag)
{
    isBSDLight_JZSY=flag;
}
bool CJZ::getIsBSDLight_JZSY()
{
    return isBSDLight_JZSY;
}
void CJZ::setIsBSDLight_JZTY(bool flag)
{
    isBSDLight_JZTY=flag;
}
bool CJZ::getIsBSDLight_JZTY()
{
    return isBSDLight_JZTY;
}
void CJZ::setIsBSDLight_JZFL(bool flag)
{
    isBSDLight_JZFL=flag;
}
bool CJZ::getIsBSDLight_JZFL()
{
    return isBSDLight_JZFL;
}
void CJZ::setIsBSDLight_JZBJ(bool flag)
{
    isBSDLight_JZBJ=flag;
}
bool CJZ::getIsBSDLight_JZBJ()
{
    return isBSDLight_JZBJ;
}
void CJZ::setIsBSDLight_QJ(bool flag)
{
    isBSDLight_QJ=flag;
}
bool CJZ::getIsBSDLight_QJ()
{
    return isBSDLight_QJ;
}
void CJZ::setArrowStateJC(int x)
{
    m_nArrowStateJC=x;
}
int CJZ::getArrowStateJC()
{
    return m_nArrowStateJC;
}
void CJZ::setArrowStateFC(int x)
{
    m_nArrowStateFC=x;
}
int CJZ::getArrowStateFC()
{
    return m_nArrowStateFC;
}
//void CJZ::DrawZDBSArrow(QPainter *painter,long nElapsed, double nDiploid,QPoint offset)
//{
//    //反走样,防止出现锯齿状线条
//    painter->setRenderHint(QPainter::Antialiasing, false);
//    /*初始化箭头数据*/
//    arrayArrowLeft[0] = QPoint((ArrowPt.x()+5*nDiploid), (ArrowPt.y()+32*nDiploid));
//    arrayArrowLeft[1] = QPoint((ArrowPt.x()+30*nDiploid), (ArrowPt.y()+32*nDiploid));
//    arrayArrowLeft[2] = QPoint((ArrowPt.x()+30*nDiploid), (ArrowPt.y()+24*nDiploid));
//    arrayArrowLeft[3] = QPoint((ArrowPt.x()+50*nDiploid), (ArrowPt.y()+36*nDiploid));
//    arrayArrowLeft[4] = QPoint((ArrowPt.x()+30*nDiploid), (ArrowPt.y()+48*nDiploid));
//    arrayArrowLeft[5] = QPoint((ArrowPt.x()+30*nDiploid), (ArrowPt.y()+40*nDiploid));
//    arrayArrowLeft[6] = QPoint((ArrowPt.x()+5*nDiploid), (ArrowPt.y()+40*nDiploid));
//    arrayArrowLeft[7] = QPoint((ArrowPt.x()+5*nDiploid), (ArrowPt.y()+32*nDiploid));

//    arrayArrowRight[0] = QPoint((ArrowPt.x()+125*nDiploid), (ArrowPt.y()+32*nDiploid));
//    arrayArrowRight[1] = QPoint((ArrowPt.x()+100*nDiploid), (ArrowPt.y()+32*nDiploid));
//    arrayArrowRight[2] = QPoint((ArrowPt.x()+100*nDiploid), (ArrowPt.y()+24*nDiploid));
//    arrayArrowRight[3] = QPoint((ArrowPt.x()+80*nDiploid), (ArrowPt.y()+36*nDiploid));
//    arrayArrowRight[4] = QPoint((ArrowPt.x()+100*nDiploid), (ArrowPt.y()+48*nDiploid));
//    arrayArrowRight[5] = QPoint((ArrowPt.x()+100*nDiploid), (ArrowPt.y()+40*nDiploid));
//    arrayArrowRight[6] = QPoint((ArrowPt.x()+125*nDiploid), (ArrowPt.y()+40*nDiploid));
//    arrayArrowRight[7] = QPoint((ArrowPt.x()+125*nDiploid), (ArrowPt.y()+32*nDiploid));

//    QPointF arrayArrowLeftForDraw[8];//左箭头
//    QPointF arrayArrowRightForDraw[8];//右箭头
//    for(int j = 0; j < 8; j++)
//    {
//        arrayArrowLeftForDraw[j] = QPoint(arrayArrowLeft[j].x(),
//            arrayArrowLeft[j].y());
//        arrayArrowRightForDraw[j] = QPoint(arrayArrowRight[j].x(),
//            arrayArrowRight[j].y());
//    }
//    QPen pen;
//    pen.setColor(GRAY);
//    pen.setWidth(1);

//    QBrush brush_Left;
//    brush_Left.setStyle(Qt::SolidPattern);
//    QColor color_Left;
//    if(m_nArrowStateJC == Arrow_Red)
//    {
//        color_Left = RED;
//    }
//    else if(m_nArrowStateJC == Arrow_Yellow)
//    {
//        color_Left = YELLOW;
//    }
//    else if(m_nArrowStateJC == Arrow_Green)
//    {
//        color_Left = GREEN;
//    }
//    else if(m_nArrowStateJC == Arrow_Black)
//    {
//        color_Left = BLACK;
//    }
//    else if(m_nArrowStateJC == Arrow_White)
//    {
//        color_Left = WHITE;
//    }
//    else if(m_nArrowStateJC == Arrow_Gray)
//    {
//        color_Left = GREY;
//    }

//    QBrush brush_Right;
//    brush_Right.setStyle(Qt::SolidPattern);
//    QColor color_Right;
//    if(m_nArrowStateFC == Arrow_Red)
//    {
//        color_Right = RED;
//    }
//    else if(m_nArrowStateFC == Arrow_Yellow)
//    {
//        color_Right = YELLOW;
//    }
//    else if(m_nArrowStateFC == Arrow_Green)
//    {
//        color_Right = GREEN;
//    }
//    else if(m_nArrowStateFC == Arrow_Black)
//    {
//        color_Right = BLACK;
//    }
//    else if(m_nArrowStateFC == Arrow_White)
//    {
//        color_Right = WHITE;
//    }
//    else if(m_nArrowStateFC == Arrow_Gray)
//    {
//        color_Right = GREY;
//    }
//    if(m_nSX)
//    {
//        brush_Left.setColor(color_Left);
//        painter->setPen(pen);
//        painter->setBrush(brush_Left);
//        painter->drawConvexPolygon(arrayArrowLeftForDraw, 8);

//        brush_Right.setColor(color_Right);
//        painter->setPen(pen);
//        painter->setBrush(brush_Right);
//        painter->drawConvexPolygon(arrayArrowRightForDraw, 8);
//    }
//    else
//    {
//        brush_Left.setColor(color_Right);
//        painter->setPen(pen);
//        painter->setBrush(brush_Left);
//        painter->drawConvexPolygon(arrayArrowLeftForDraw, 8);

//        brush_Right.setColor(color_Left);
//        painter->setPen(pen);
//        painter->setBrush(brush_Right);
//        painter->drawConvexPolygon(arrayArrowRightForDraw, 8);
//    }

//}

void CJZ::DrawQFRect(QPainter *painter,QPointF QFPoint, int m_nQFNumber, long nElapsed, double nDiploid,QPoint offset)
{
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);

    QRect m_rectQF;

    m_rectQF = QRect(0,0,0,0);

    //初始化铅封框
    m_rectQF = QRect(QFPoint.x(),
                     QFPoint.y(),
                     25*nDiploid,
                     14*nDiploid);

    QPen pen;
    pen.setColor(YELLOW);
    pen.setWidth(1);
    painter->setPen(pen);
    QBrush newBrushBK;
    newBrushBK.setStyle(Qt::NoBrush);
    painter->setBrush(newBrushBK);
    painter->drawRect(m_rectQF);

    pen.setColor(WHITE);
    painter->setPen(pen);
    if(m_nQFNumber == 0)
    {
        painter->drawText(m_rectQF,"");
    }
    else if(m_nQFNumber > 0 && m_nQFNumber < 10)
    {
        QString str = QString("%1").arg(m_nQFNumber);
        painter->drawText(m_rectQF,Qt::AlignHCenter|Qt::AlignVCenter,"00" + str);
    }
    else if(m_nQFNumber >9 && m_nQFNumber < 100)
    {
        QString str = QString("%1").arg(m_nQFNumber);
        painter->drawText(m_rectQF,Qt::AlignHCenter|Qt::AlignVCenter,"0" + str);
    }
    else
    {
        QString str = QString("%1").arg(m_nQFNumber);
        painter->drawText(m_rectQF,Qt::AlignHCenter|Qt::AlignVCenter,str);
    }

}

void CJZ::Draw_circle_Button(QPainter *painter,//画笔
                              QRectF m_ButtonRect,//按钮绘制区域
                              bool m_FCZKState,//非常站控状态
                              int m_ButtonIsDownState,//按钮是否按下
                              bool m_FSState,//封锁状态
                              bool m_bZDANFlash,//终端闪烁状态
                              int mTimeCount,//倒计时时间
                              QString rectTextShow, //按钮文字显示
                              bool RangeVisible,//按钮是否高亮
                              long nElapsed, //闪烁读秒
                              double nDiploid, //放大倍数
                              QPoint offset)//绘制圆形按钮
{
#pragma region//>..调车按钮绘制
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);

    //按钮边框默认颜色
    QColor btnClrTopLeft = WHITE;//按钮边框-TL
    QColor btnClrBottomRight = GRAY;//按钮边框-BR
//    if(m_RangeVisible)
//    {
//        btnClrTopLeft = THIN_BLUE_DOWN;//按钮边框-TL
//        btnClrBottomRight = THIN_BLUE_DOWN;//按钮边框-BR
//    }
    //按钮动作正在按下
    if (0<m_ButtonIsDownState && m_ButtonIsDownState<4)
    {
        if (nElapsed % 2 == 0)
        {
            btnClrTopLeft = WHITE;//按钮边框-TL
            btnClrBottomRight = GRAY;//按钮边框-BR
        }
        else
        {
            btnClrTopLeft = GRAY;//按钮边框-TL
            btnClrBottomRight = WHITE;//按钮边框-BR
        }
    }
    //绘制下半圆
    QPen pen;
    pen.setColor(btnClrBottomRight);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawArc(m_ButtonRect, 180*16, 180*16);
    //绘制上半圆
    pen.setColor(btnClrTopLeft);
    painter->setPen(pen);
    painter->drawArc(m_ButtonRect, 0, 180*16);

    //绘制调车按钮圆体
    if(m_FCZKState)//非常站控模式下
    {
        QBrush newBrushBK;
        newBrushBK.setStyle(Qt::SolidPattern);
        if(m_ButtonIsDownState == 2)
        {
            newBrushBK.setColor(RED);
            painter->setBrush(newBrushBK);

            pen.setColor(RED);
            painter->setPen(pen);
        }
        else
        {
            newBrushBK.setColor(THIN_GRAY);
            painter->setBrush(newBrushBK);

            pen.setColor(THIN_GRAY);
            painter->setPen(pen);
        }
    }
    else if(m_FSState)//按钮封锁-粉闪
    {
        if(nElapsed % XHDBTN_FLASTIME == 0) //控制时间间隔
        {
            if(m_ButtonIsDownState == 1)
            {
                QBrush newBrushBK;
                newBrushBK.setStyle(Qt::SolidPattern);
                newBrushBK.setColor(BLUE);
                painter->setBrush(newBrushBK);

                pen.setColor(BLUE);
                painter->setPen(pen);
            }
            else
            {
                QBrush newBrushBK;
                newBrushBK.setStyle(Qt::SolidPattern);
                newBrushBK.setColor(THIN_GRAY);
                painter->setBrush(newBrushBK);

                pen.setColor(THIN_GRAY);
                painter->setPen(pen);
            }
        }
        else
        {
            QBrush newBrushBK;
            newBrushBK.setStyle(Qt::SolidPattern);
            newBrushBK.setColor(BROWN);
            painter->setBrush(newBrushBK);

            pen.setColor(BROWN);
            painter->setPen(pen);
        }
    }
    else if(RangeVisible)
    {
        QBrush newBrushBK;
        newBrushBK.setStyle(Qt::SolidPattern);
        newBrushBK.setColor(THIN_BLUE_DOWN);
        painter->setBrush(newBrushBK);

        pen.setColor(THIN_BLUE_DOWN);
        painter->setPen(pen);
    }
    else
    {
        switch (m_ButtonIsDownState)
        {
        case 0:
            {
                QBrush newBrushBK;
                newBrushBK.setStyle(Qt::SolidPattern);
                newBrushBK.setColor(THIN_GRAY);
                painter->setBrush(newBrushBK);

                pen.setColor(THIN_GRAY);
                painter->setPen(pen);
                break;
            }
        case 1:
            {
                if(nElapsed % XHDBTN_FLASTIME == 0) //控制时间间隔
                {
                    QBrush newBrushBK;
                    newBrushBK.setStyle(Qt::SolidPattern);
                    newBrushBK.setColor(WHITEX);
                    painter->setBrush(newBrushBK);

                    pen.setColor(WHITEX);
                    painter->setPen(pen);
                }
                else
                {
                    QBrush newBrushBK;
                    newBrushBK.setStyle(Qt::SolidPattern);
                    newBrushBK.setColor(BLUE);
                    painter->setBrush(newBrushBK);

                    pen.setColor(BLUE);
                    painter->setPen(pen);
                }
                break;
            }
        case 2:
            {
                QBrush newBrushBK;
                newBrushBK.setStyle(Qt::SolidPattern);
                newBrushBK.setColor(RED);
                painter->setBrush(newBrushBK);

                pen.setColor(RED);
                painter->setPen(pen);
                break;
            }
        case 4://可按下，终端闪烁
            {
                if(m_bZDANFlash)
                {
                    if(nElapsed % XHDBTN_FLASTIME == 0) //控制时间间隔
                    {
                        QBrush newBrushBK;
                        newBrushBK.setStyle(Qt::SolidPattern);
                        newBrushBK.setColor(YELLOW);
                        painter->setBrush(newBrushBK);

                        pen.setColor(YELLOW);
                        painter->setPen(pen);
                    }
                    else
                    {
                        QBrush newBrushBK;
                        newBrushBK.setStyle(Qt::SolidPattern);
                        newBrushBK.setColor(GREY);
                        painter->setBrush(newBrushBK);

                        pen.setColor(GREY);
                        painter->setPen(pen);
                    }
                }
                else
                {
                    QBrush newBrushBK;
                    newBrushBK.setStyle(Qt::SolidPattern);
                    newBrushBK.setColor(GREY);
                    painter->setBrush(newBrushBK);

                    pen.setColor(GREY);
                    painter->setPen(pen);
                }
                break;
            }
        }
    }
    QPointF point = QPointF(m_ButtonRect.left()+7*nDiploid,m_ButtonRect.top()+7*nDiploid);
    painter->drawEllipse(point,5 * nDiploid,5 * nDiploid);

    //封锁X
    if(m_FSState)
    {
        QPen penX;
        penX.setColor(RED);
        penX.setWidth(2);
        painter->setPen(penX);

        QRectF m_rectDC_X;       //调车按钮区域
        m_rectDC_X.setLeft(m_ButtonRect.left());
        m_rectDC_X.setTop(m_ButtonRect.top());
        m_rectDC_X.setRight(m_ButtonRect.right());
        m_rectDC_X.setBottom(m_ButtonRect.bottom());

        painter->drawLine(m_rectDC_X.left(), m_rectDC_X.top(),m_rectDC_X.right(), m_rectDC_X.bottom());
        painter->drawLine(m_rectDC_X.right(), m_rectDC_X.top(),m_rectDC_X.left(), m_rectDC_X.bottom());
    }
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);

    //倒计时
    if((0<mTimeCount)&&(255>mTimeCount))
    {
        QString strTimeCount;
        strTimeCount=QString::number(mTimeCount);

        QPen penX;
        penX.setColor(RED);
        penX.setWidth(1);
        painter->setPen(penX);
        QRectF m_rectDC_X;
        m_rectDC_X.setLeft(m_ButtonRect.left()-20 * nDiploid);
        m_rectDC_X.setTop(m_ButtonRect.top()-20 * nDiploid);
        m_rectDC_X.setRight(m_ButtonRect.right()+20 * nDiploid);
        m_rectDC_X.setBottom(m_ButtonRect.bottom()-20 * nDiploid);

        painter->drawText(m_rectDC_X,Qt::AlignHCenter|Qt::AlignVCenter,strTimeCount);
    }
    //绘制按钮的文字
    if(rectTextShow != ""&&bBtnNameShow)
    {
        pen.setColor(YELLOW);
        QFont font;
        font.setWeight(50);//字体宽度,0~99
        font.setPointSize(BSWordSize * nDiploid);//字体高度,10表示高度为10个像素点
        painter->setPen(pen);
        painter->setFont(font);
        QRectF rectBtnWord_X;
        rectBtnWord_X.setLeft(m_ButtonRect.left()-30 * nDiploid);
        rectBtnWord_X.setTop(m_ButtonRect.top()+15 * nDiploid);
        rectBtnWord_X.setRight(m_ButtonRect.right()+30 * nDiploid);
        rectBtnWord_X.setBottom(m_ButtonRect.bottom()+25 * nDiploid);
//        painter->drawText(rectBtnTG_X,Qt::AlignHCenter|Qt::AlignVCenter,m_strName + "TA");
        painter->drawText(rectBtnWord_X,Qt::AlignHCenter|Qt::AlignVCenter,rectTextShow);
    }
#pragma endregion
}

void CJZ::Draw_Rect_Button(QPainter *painter,//画笔
                              QRectF m_ButtonRect,//按钮绘制区域
                              bool m_FCZKState,//非常站控状态
                              int m_ButtonIsDownState,//按钮是否按下
                              bool m_FSState,//封锁状态
                              bool m_bZDANFlash,//终端闪烁状态
                              int mTimeCount,//倒计时时间
                              QColor color,//按钮颜色
                              QString rectTextShow, //按钮文字显示
                              bool RangeVisible,//按钮是否高亮
                              long nElapsed,//闪烁读秒
                              double nDiploid,//放大倍数
                              QPoint offset)//绘制KSK方形按钮
{
#pragma region//>..列车按钮绘制
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);

    //按钮边框默认颜色
    QColor btnClrTopLeft = WHITE;//按钮边框-TL
    QColor btnClrBottomRight = GRAY ;//按钮边框-BR
    //绘制按钮边框
    QPen pen_bk;
    pen_bk.setColor(btnClrBottomRight);
    pen_bk.setWidth(2);
    painter->setPen(pen_bk);
    painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.bottom()+2);
    painter->drawLine(m_ButtonRect.right()+2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);

    pen_bk.setColor(btnClrTopLeft);
    pen_bk.setWidth(2);
    painter->setPen(pen_bk);
    painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.left()-2,m_ButtonRect.top()-2);
    painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.top()-2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);

//    if(m_RangeVisible)
//    {
//        btnClrTopLeft = THIN_BLUE_DOWN;//按钮边框-TL
//        btnClrBottomRight = THIN_BLUE_DOWN;//按钮边框-BR
//    }
    //按钮动作正在按下
//    if(!m_FCZKState)
    if(1)
    {
        if (0<m_ButtonIsDownState && m_ButtonIsDownState<4)
        {
            if (nElapsed % 2 == 0)
            {
                btnClrTopLeft = WHITE;//按钮边框-TL
                btnClrBottomRight = GRAY;//按钮边框-BR

                pen_bk.setColor(btnClrBottomRight);
                pen_bk.setWidth(2);
                painter->setPen(pen_bk);
                painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.bottom()+2);
                painter->drawLine(m_ButtonRect.right()+2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);

                pen_bk.setColor(btnClrTopLeft);
                pen_bk.setWidth(2);
                painter->setPen(pen_bk);
                painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.left()-2,m_ButtonRect.top()-2);
                painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.top()-2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);

            }
            else
            {
                btnClrTopLeft = GRAY;//按钮边框-TL
                btnClrBottomRight = WHITE;//按钮边框-BR

                pen_bk.setColor(btnClrTopLeft);
                pen_bk.setWidth(2);
                painter->setPen(pen_bk);
                painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.left()-2,m_ButtonRect.top()-2);
                painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.top()-2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);

                pen_bk.setColor(btnClrBottomRight);
                pen_bk.setWidth(2);
                painter->setPen(pen_bk);
                painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.bottom()+2);
                painter->drawLine(m_ButtonRect.right()+2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);
            }
        }
        if (m_ButtonIsDownState==2)
        {
            btnClrTopLeft = GRAY;//按钮边框-TL
            btnClrBottomRight = WHITE;//按钮边框-BR

            pen_bk.setColor(btnClrTopLeft);
            pen_bk.setWidth(2);
            painter->setPen(pen_bk);
            painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.left()-2,m_ButtonRect.top()-2);
            painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.top()-2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);

            pen_bk.setColor(btnClrBottomRight);
            pen_bk.setWidth(2);
            painter->setPen(pen_bk);
            painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.bottom()+2);
            painter->drawLine(m_ButtonRect.right()+2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);
        }
        if(RangeVisible)
        {
            btnClrTopLeft = WHITE;//按钮边框-TL
            btnClrBottomRight = GRAY ;//按钮边框-BR

            pen_bk.setColor(btnClrBottomRight);
            pen_bk.setWidth(2);
            painter->setPen(pen_bk);
            painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.bottom()+2);
            painter->drawLine(m_ButtonRect.right()+2,m_ButtonRect.bottom()+2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);

            pen_bk.setColor(btnClrTopLeft);
            pen_bk.setWidth(2);
            painter->setPen(pen_bk);
            painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.bottom()+2,m_ButtonRect.left()-2,m_ButtonRect.top()-2);
            painter->drawLine(m_ButtonRect.left()-2,m_ButtonRect.top()-2,m_ButtonRect.right()+2,m_ButtonRect.top()-2);
        }
    }


    QBrush newBrushBK;
    newBrushBK.setStyle(Qt::SolidPattern);
    QPen pen;
    pen.setWidth(2);
    pen.setJoinStyle(Qt::PenJoinStyle ::MiterJoin);
    //绘制按钮主体
//    if(m_FCZKState)//非常站控模式下
    if(0)//非常站控模式下
    {
        if(m_ButtonIsDownState == 2)
        {
            newBrushBK.setColor(RED);
            painter->setBrush(newBrushBK);

            pen.setColor(RED);
            painter->setPen(pen);
        }
        else
        {
            newBrushBK.setColor(THIN_GRAY);
            painter->setBrush(newBrushBK);

            pen.setColor(THIN_GRAY);
            painter->setPen(pen);
        }
    }
    else if(m_FSState)//按钮封锁-粉闪
    {
        if(nElapsed % XHDBTN_FLASTIME == 0) //控制时间间隔
        {
            if(m_ButtonIsDownState == 1)
            {
                newBrushBK.setColor(BLUE);
                painter->setBrush(newBrushBK);

                pen.setColor(BLUE);
                painter->setPen(pen);
            }
            else
            {
                newBrushBK.setColor(THIN_GRAY);
                painter->setBrush(newBrushBK);

                pen.setColor(THIN_GRAY);
                painter->setPen(pen);
            }
        }
        else
        {
            newBrushBK.setColor(BROWN);
            painter->setBrush(newBrushBK);

            pen.setColor(BROWN);
            painter->setPen(pen);
        }
    }
    else if(RangeVisible)
    {
        newBrushBK.setColor(THIN_BLUE_DOWN);
        painter->setBrush(newBrushBK);

        pen.setColor(THIN_BLUE_DOWN);
        painter->setPen(pen);
    }
    else
    {
        switch (m_ButtonIsDownState)
        {
        case 0:
            {
                newBrushBK.setColor(color);
                painter->setBrush(newBrushBK);

                pen.setColor(color);
                painter->setPen(pen);
                break;
            }
        case 1:
            {
                if(nElapsed % XHDBTN_FLASTIME == 0) //控制时间间隔
                {
                    newBrushBK.setColor(WHITEX);
                    painter->setBrush(newBrushBK);

                    pen.setColor(WHITEX);
                    painter->setPen(pen);
                }
                else
                {
                    newBrushBK.setColor(BLUE);
                    painter->setBrush(newBrushBK);

                    pen.setColor(BLUE);
                    painter->setPen(pen);
                }
                break;
            }
        case 2:
            {
                newBrushBK.setColor(RED);
                painter->setBrush(newBrushBK);

                pen.setColor(RED);
                painter->setPen(pen);
                break;
            }
        case 4://可按下，终端闪烁
            {
                if(m_bZDANFlash)
                {
                    if(nElapsed % XHDBTN_FLASTIME == 0) //控制时间间隔
                    {
                        newBrushBK.setColor(YELLOW);
                        painter->setBrush(newBrushBK);

                        pen.setColor(YELLOW);
                        painter->setPen(pen);
                    }
                    else
                    {
                        newBrushBK.setColor(GREY);
                        painter->setBrush(newBrushBK);

                        pen.setColor(GREY);
                        painter->setPen(pen);
                    }
                }
                else
                {
                    newBrushBK.setColor(GREY);
                    painter->setBrush(newBrushBK);

                    pen.setColor(GREY);
                    painter->setPen(pen);
                }
                break;
            }
        }
    }
    painter->drawRect(m_ButtonRect);

    //封锁X
    if(m_FSState)
    {
        QPen penX;
        penX.setColor(RED);
        penX.setWidth(2);
        painter->setPen(penX);

        QRectF m_rectDC_X;       //调车按钮区域
        m_rectDC_X.setLeft(m_ButtonRect.left());
        m_rectDC_X.setTop(m_ButtonRect.top());
        m_rectDC_X.setRight(m_ButtonRect.right());
        m_rectDC_X.setBottom(m_ButtonRect.bottom());

        painter->drawLine(m_rectDC_X.left(), m_rectDC_X.top(),m_rectDC_X.right(), m_rectDC_X.bottom());
        painter->drawLine(m_rectDC_X.right(), m_rectDC_X.top(),m_rectDC_X.left(), m_rectDC_X.bottom());
    }
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);
    //倒计时
    if((0<mTimeCount)&&(255>mTimeCount))
    {
        QString strTimeCount;
        strTimeCount=QString::number(mTimeCount);

        QPen penX;
        penX.setColor(RED);
        penX.setWidth(1);
        painter->setPen(penX);
        QRectF m_rectDC_X;
        m_rectDC_X.setLeft(m_ButtonRect.left()-20 * nDiploid);
        m_rectDC_X.setTop(m_ButtonRect.top()-20 * nDiploid);
        m_rectDC_X.setRight(m_ButtonRect.right()+20 * nDiploid);
        m_rectDC_X.setBottom(m_ButtonRect.bottom()-20 * nDiploid);

        painter->drawText(m_rectDC_X,Qt::AlignHCenter|Qt::AlignVCenter,strTimeCount);
    }
    //绘制按钮的文字
    if(rectTextShow != ""&&bBtnNameShow)
    {
        pen.setColor(YELLOW);
        QFont font;
        font.setWeight(50);//字体宽度,0~99
        font.setPointSize(BSWordSize * nDiploid);//字体高度,10表示高度为10个像素点
        painter->setPen(pen);
        painter->setFont(font);
        QRectF rectBtnWord_X;
        rectBtnWord_X.setLeft(m_ButtonRect.left()-30 * nDiploid);
        rectBtnWord_X.setTop(m_ButtonRect.top()+15 * nDiploid);
        rectBtnWord_X.setRight(m_ButtonRect.right()+30 * nDiploid);
        rectBtnWord_X.setBottom(m_ButtonRect.bottom()+25 * nDiploid);
//        painter->drawText(rectBtnTG_X,Qt::AlignHCenter|Qt::AlignVCenter,m_strName + "TA");
        painter->drawText(rectBtnWord_X,Qt::AlignHCenter|Qt::AlignVCenter,rectTextShow);
    }
    if(RangeVisible)
    {
        QRectF textRect;
        textRect.setLeft(m_ButtonRect.left()-4 * nDiploid);
        textRect.setTop(m_ButtonRect.top()-4 * nDiploid);
        textRect.setRight(m_ButtonRect.right()+4);
        textRect.setBottom(m_ButtonRect.bottom()+4 * nDiploid);
        painter->setPen(QPen(Qt::white, 1,Qt::DashLine));
        painter->setBrush(THIN_BLUE_DOWN);
        painter->setBrush(Qt::transparent);
        painter->drawRect(textRect);
        painter->setPen(BLACK);
    }

#pragma endregion
}

void CJZ::Draw_Rect_Light(QPainter *painter,//画笔
                            QPointF LightPoint,//指示灯中心坐标
                            QString rectTextShow, //文字显示
                            bool Light_color,//指示灯状态
                            int light_type,//指示灯类型
                            long nElapsed,//闪烁读秒
                            double nDiploid,//放大倍数
                            QPoint offset)//绘制指示灯
{
#pragma region//>..绘制自动闭塞灯
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);

    QPointF CenterPoint;
    QBrush newBrushBK;
    newBrushBK.setStyle(Qt::SolidPattern);
    QColor color_Light;
    if(Light_color)
    {
        //color_Light = getLightState(2,nElapsed);

        if(rectTextShow.contains("区间")||rectTextShow.contains("区轨"))
        {
            if(rectTextShow == QJD_Name)
            {
                if(isBSDLight_JZSY == true)
                {
                    color_Light = getLightState(light_type,nElapsed);
                }
                else
                {
                    color_Light = getLightState(0,nElapsed);
                }
            }
        }
        else
        {
            color_Light = getLightState(light_type,nElapsed);
        }

    }
    else
    {
        if(rectTextShow.contains("区间")||rectTextShow.contains("区轨"))
        {
            if(rectTextShow == QJD_Name)
            {
                if(isBSDLight_JZSY == true)
                {
                    color_Light = getLightState(3,nElapsed);
                }
                else
                {
                    color_Light = getLightState(0,nElapsed);
                }
            }
        }
        else
        {
            color_Light = getLightState(0,nElapsed);
        }
    }
    newBrushBK.setColor(color_Light);
    painter->setBrush(newBrushBK);

    QPen pen;
    pen.setColor(THIN_GRAY);
    painter->setPen(pen);

    CenterPoint = QPointF(LightPoint.x(), LightPoint.y());
    painter->drawEllipse(CenterPoint, 8 * nDiploid, 8 * nDiploid);

    //绘制指示灯的文字
    if(rectTextShow != ""&&bBtnNameShow)
    {
        pen.setColor(WHITE);
        QFont font;
        font.setWeight(50);//字体宽度,0~99
        //font.setPointSize(12 * nDiploid);//字体高度,10表示高度为10个像素点
        font.setPointSize(BSWordSize * nDiploid);
        painter->setPen(pen);
        painter->setFont(font);
        QRectF rectBtnWord_X;
        rectBtnWord_X.setLeft(LightPoint.x()-32 * nDiploid);
        rectBtnWord_X.setTop(LightPoint.y()+12 * nDiploid);
        rectBtnWord_X.setRight(LightPoint.x()+32 * nDiploid);
        rectBtnWord_X.setBottom(LightPoint.y()+32 * nDiploid);
        painter->drawText(rectBtnWord_X,Qt::AlignHCenter|Qt::AlignVCenter,rectTextShow);
        //painter->drawText(rectBtnWord_X, rectTextShow);
    }
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);
#pragma endregion
}

QColor CJZ::getLightState(int nType,int nElapsed)//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝）
{
    QColor color;
    color = BLACK;
    switch(nType)
    {
    case 0:
        {
            color = BLACK;
            return color;
            break;
        }
    case 1:
        {
            color = RED;
            return color;
            break;
        }
    case 2:
        {
            color = GREEN;
            return color;
            break;
        }
    case 3:
        {
            color = WHITE;
            return color;
            break;
        }
    case 4:
        {
            color = YELLOW;
            return color;
            break;
        }
    case 5:
        {
            if(nElapsed%2 == 0)
                color = RED;
            else
                color = BLACK;
            return color;
            break;
        }
    case 6:
        {
            if(nElapsed%2 == 0)
                color = GREEN;
            else
                color = BLACK;
            return color;
            break;
        }
    case 7:
        {
            if(nElapsed%2 == 0)
                color = WHITE;
            else
                color = BLACK;
            return color;
            break;
        }
    case 8:
        {
            if(nElapsed%2 == 0)
                color = YELLOW;
            else
                color = BLACK;
            return color;
            break;
        }
    case 9:
        {
            color = BLUE;
            return color;
            break;
        }
    case 10:
        {
            color = THIN_GRAY;
            return color;
            break;
        }

    }
    return color;
}

bool CJZ::GetMousePoint_BSQH(QPoint p)
{
    if (rect_BSQH.contains(p))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool CJZ::GetMousePoint_JZSY(QPoint p)
{
    if (rect_JZSY.contains(p))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool CJZ::GetMousePoint_JZTY(QPoint p)
{
    if (rect_JZTY.contains(p))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool CJZ::GetMousePoint_JZFL(QPoint p)
{
    if (rect_JZFL.contains(p))
    {
        return true;
    }
    else
    {
        return false;
    }
}
QString CJZ::GetANIsDown(int nType)
{
    QString strName="";
    if(nType==1)//使用
    {
         strName="计轴使用";
    }
    if(nType==2)//停用
    {
        strName="计轴停用";
    }
    if(nType==3)//复零
    {
        strName="计轴复零";
    }
    if(nType==4)//闭塞切换按钮
    {
         strName="闭塞切换";
    }
    return strName;
}
void CJZ::SetANIsDown(int nType)
{
    QString strName="";
    if(nType==1)//使用
    {
        setIsDown_JZSY(1);
         strName="计轴使用";
    }
    else
    {
        setIsDown_JZSY(0);
    }
    if(nType==2)//停用
    {
       setIsDown_JZTY(1);
        strName="计轴停用";
    }
    else
    {
        setIsDown_JZTY(0);
    }
    if(nType==3)//复零
    {
        setIsDown_JZFL(1);
        strName="计轴复零";
    }
    else
    {
        setIsDown_JZFL(0);
    }
    if(nType==4)//闭塞切换按钮
    {
        setIsDown_BSQH(1);
         strName="闭塞切换";
    }
    else
    {
        setIsDown_BSQH(0);
    }
}










