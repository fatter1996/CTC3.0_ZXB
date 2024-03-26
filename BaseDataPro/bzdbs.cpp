#include "bzdbs.h"

CBZDBS::CBZDBS()
{
    m_nGLXH=0xffff;
    isNX=0;
    ArrowPt=QPoint(0,0);
    ArrowPtConst=QPoint(0,0);
    m_nAnNum=0;
    p_BSQHAN=QPoint(0,0);
    p_BSQHANConst=QPoint(0,0);
    p_BSAN=QPoint(0,0);
    p_BSANConst=QPoint(0,0);
    p_SGAN=QPoint(0,0);
    p_SGANConst=QPoint(0,0);
    p_FYAN=QPoint(0,0);
    p_FYANConst=QPoint(0,0);
    p_MN=QPoint(0,0);
    p_MNConst=QPoint(0,0);
    rect_BSQH=QRect(0,0,0,0);
    rect_BS=QRect(0,0,0,0);
    rect_SG=QRect(0,0,0,0);
    rect_FY=QRect(0,0,0,0);
    isDown_BSQH=false;
    isDown_BS=false;
    isDown_SG=false;
    isDown_FY=false;
//    m_nDownTime_FCFZ=0xFF;
//    m_nDownTime_JCFZ=0xFF;
    p_BSD_ZY=QPoint(0,0);
    p_BSD_ZYConst=QPoint(0,0);
    p_BSD_KX=QPoint(0,0);
    p_BSD_KXConst=QPoint(0,0);
    p_BSD_RGBS=QPoint(0,0);
    p_BSD_RGBSConst=QPoint(0,0);
    p_BSD_ZDBS=QPoint(0,0);
    p_BSD_ZDBSConst=QPoint(0,0);
    rect_FK=QRect(0,0,0,0);
    rect_FKConst=QRect(0,0,0,0);
    isBSDLight_ZY=false;
    isBSDLight_RGBS=false;
    isBSDLight_KX=false;
    isBSDLight_ZDBS=false;
    m_nArrowStateJC=Arrow_Black;
    m_nArrowStateFC=Arrow_Black;

    BSQHAN_Name = "";
    BSAN_Name = "";
    SGAN_Name = "";
    FYAN_Name = "";
    ZYD_Name = "";
    KXD_Name = "";
    RGBSD_Name = "";
    ZDBSD_Name = "";
    ArrowLeftName = "";
    ArrowRightName = "";

    haveBSQHAN = false;
    haveBSAN = false;
    haveSGAN = false;
    haveFYAN = false;
    haveZYD = false;
    haveKXD = false;
    haveRGBSD = false;
    haveZDBSD = false;

    BSQHAN_QF = 0;
    BSAN_QF = 0;
    SGAN_QF = 0;
    FYAN_QF = 0;

    haveBSQHAN_QF = false;
    haveBSAN_QF = false;
    haveSGAN_QF = false;
    haveFYAN_QF = false;
    haveBZDBS_QF = false;

    m_RangeVisible = false;
    ButtonForm = 1;

    haveFK = false;
    ptFK=QPoint(0,0);
    FK_length = 0;
    FK_breath = 0;

    haveWord = false;
    ptWord = QPoint(0,0);
    WordName = "";

    m_RangeVisible_BZDBS_BSQH = false; //设置设备范围显示（或高亮显示）//点灯灭灯按下
    m_RangeVisible_BZDBS_BS = false; //设置设备范围显示（或高亮显示）//点灯灭灯按下
    m_RangeVisible_BZDBS_SG = false; //设置设备范围显示（或高亮显示）//点灯灭灯按下
    m_RangeVisible_BZDBS_FY = false; //设置设备范围显示（或高亮显示）//点灯灭灯按下

}
void CBZDBS::Draw_ToolTip(QPainter *painter, double nDiploid)
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
int CBZDBS::moveCursor(QPoint p)
{
    if (rect_BS.contains(p))
    {
        return 1;
    }
    else if(rect_SG.contains(p))
    {
        return 2;
    }
    else if(rect_FY.contains(p))
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
void CBZDBS::Draw(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type)
{
    if(type==0x55)
    {
        Draw_Th_kb(painter,nElapsed, nDiploid,offset, false);
    }
}

void CBZDBS::DrawM(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type,int flag)
{
    if(type==0x55)
    {
        Draw_Th_kb(painter,nElapsed, nDiploid,offset, true);
    }
}

void CBZDBS::Draw_Th_kb(QPainter *painter,long nElapsed, double nDiploid,QPoint offset, bool isMulti)
{
    QFont font;
    ArrowPt.setX(ArrowPtConst.x() * nDiploid+offset.x()*nDiploid);
    ArrowPt.setY(ArrowPtConst.y() * nDiploid+offset.y()*nDiploid);

    if(ButtonForm == 1)
    {
        rect_BSQH.setRect(p_BSQHANConst.x() * nDiploid + offset.x() * nDiploid,p_BSQHANConst.y() * nDiploid + offset.y() * nDiploid,10*nDiploid,10*nDiploid);
        rect_BS.setRect(p_BSANConst.x() * nDiploid + offset.x() * nDiploid,p_BSANConst.y() * nDiploid + offset.y() * nDiploid,10*nDiploid,10*nDiploid);
        rect_SG.setRect(p_SGANConst.x() * nDiploid + offset.x() * nDiploid,p_SGANConst.y() * nDiploid + offset.y() * nDiploid,10*nDiploid,10*nDiploid);
        rect_FY.setRect(p_FYANConst.x() * nDiploid + offset.x() * nDiploid,p_FYANConst.y() * nDiploid + offset.y() * nDiploid,10*nDiploid,10*nDiploid);
    }
    else
    {
        rect_BSQH.setRect(p_BSQHANConst.x() * nDiploid + offset.x() * nDiploid,p_BSQHANConst.y() * nDiploid + offset.y() * nDiploid,14*nDiploid,14*nDiploid);
        rect_BS.setRect(p_BSANConst.x() * nDiploid + offset.x() * nDiploid,p_BSANConst.y() * nDiploid + offset.y() * nDiploid,14*nDiploid,14*nDiploid);
        rect_SG.setRect(p_SGANConst.x() * nDiploid + offset.x() * nDiploid,p_SGANConst.y() * nDiploid + offset.y() * nDiploid,14*nDiploid,14*nDiploid);
        rect_FY.setRect(p_FYANConst.x() * nDiploid + offset.x() * nDiploid,p_FYANConst.y() * nDiploid + offset.y() * nDiploid,14*nDiploid,14*nDiploid);
    }

    p_BSD_KX.setX(p_BSD_KXConst.x()  * nDiploid+offset.x()*nDiploid);
    p_BSD_KX.setY(p_BSD_KXConst.y()  * nDiploid+offset.y()*nDiploid);
    p_BSD_RGBS.setX(p_BSD_RGBSConst.x()  * nDiploid+offset.x()*nDiploid);
    p_BSD_RGBS.setY(p_BSD_RGBSConst.y()  * nDiploid+offset.y()*nDiploid);
    p_BSD_ZDBS.setX(p_BSD_ZDBSConst.x()  * nDiploid+offset.x()*nDiploid);
    p_BSD_ZDBS.setY(p_BSD_ZDBSConst.y()  * nDiploid+offset.y()*nDiploid);
    p_BSD_ZY.setX(p_BSD_ZYConst.x()  * nDiploid+offset.x()*nDiploid);
    p_BSD_ZY.setY(p_BSD_ZYConst.y()  * nDiploid+offset.y()*nDiploid);
//    rect_FK.setRect(rect_FKConst.topLeft().x() * nDiploid + offset.x() * nDiploid,rect_FKConst.topLeft().y() * nDiploid + offset.y() * nDiploid,rect_FKConst.width()*nDiploid,rect_FKConst.height()*nDiploid);
    rect_FK.setRect(ptFK.x() * nDiploid + offset.x() * nDiploid,ptFK.y() * nDiploid + offset.y() * nDiploid,FK_length*nDiploid,FK_breath*nDiploid);
    int WordNameLenth;
    WordNameLenth = WordName.length();
    rect_Word.setRect(ptWord.x() * nDiploid + offset.x() * nDiploid,ptWord.y() * nDiploid + offset.y() * nDiploid,WordNameLenth*14*nDiploid,20*nDiploid);
    //rect_Word.setRect(ptWord.x() * nDiploid + offset.x() * nDiploid,ptWord.y() * nDiploid + offset.y() * nDiploid,WordNameLenth*12*nDiploid,20*nDiploid);

    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);

    font.setFamily("宋体");
    font.setPointSize(10 * nDiploid);//字号
    font.setItalic(false);//斜体
    painter->setFont(font);//设置字体
    painter->setPen(Qt::white);//设置画笔颜色

    QString strNameAN[4]={"总辅助","发车辅助","接车辅助","改方按钮"};
    bool isDownAN[4]={false,false,false,false};
    isDownAN[0]=getIsDown_BS();
    isDownAN[1]=getIsDown_SG();
    isDownAN[2]=getIsDown_FY();
    isDownAN[3]=getIsDown_BSQH();
    QRect rectAN[4]={rect_BS,rect_SG,rect_FY,rect_BSQH};

    QString strNameBSD[4]={"允许发车","辅助","闪光","区间"};
    bool isLight[4]={false,false,false,false};
    isLight[0]=getIsBSDLight_ZY();
    isLight[1]=getIsBSDLight_RGBS();
    isLight[2]=getIsBSDLight_KX();
    isLight[3]=getIsBSDLight_ZDBS();
    QPoint bsdPt[4]={p_BSD_ZY,p_BSD_RGBS,p_BSD_KX,p_BSD_ZDBS};

    DrawZDBSArrow(painter,nElapsed,nDiploid,offset);//绘制箭头
    if(isMulti) return;
    if(/*VISIBLE_JSQ*/0)
    {//绘制自动闭塞铅封
        QPoint ArrowQF;
        ArrowQF.setX(ArrowPt.x()+56*nDiploid);
        ArrowQF.setY(ArrowPt.y()+28*nDiploid);
        DrawQFRect(painter,ArrowQF, BZDBS_QF, nElapsed,nDiploid,offset);
    }
    if(haveBSAN)
    {
        haveBSAN_QF = false;
        if(ButtonForm == 1)
        {
            Draw_Rect_Button(painter,//画笔
                             rect_BS,//按钮绘制区域//需要修改按钮区域大小为10
                             bFCZKState,//非常站控状态
                             getIsDown_BS(),//按钮是否按下
                             false,//封锁状态
                             false,//终端闪烁状态
                             0,//倒计时时间
                             THIN_GRAY,//按钮颜色
                             haveBSAN_QF,//铅封
                             BSAN_Name, //按钮文字显示
                             m_RangeVisible_BZDBS_BS,//按钮是否高亮
                             nElapsed,//闪烁读秒
                             nDiploid,//放大倍数
                             offset);//绘制KSK方形按钮
        }
        else
        {
            Draw_circle_Button(painter,//画笔
                               rect_BS,//按钮绘制区域//需要修改按钮区域大小为14
                               bFCZKState,//非常站控状态
                               getIsDown_BS(),//按钮是否按下
                               false,//封锁状态
                               false,//终端闪烁状态
                               0,//倒计时时间
                               haveBSAN_QF,//铅封
                               BSAN_Name, //按钮文字显示
                               m_RangeVisible_BZDBS_BS,//按钮是否高亮
                               nElapsed, //闪烁读秒
                               nDiploid, //放大倍数
                               offset);//绘制圆形按钮
        }
        //if(VISIBLE_JSQ)
//        {
//            QPoint ZFZQF;
//            ZFZQF.setX(rect_BS.x()-4*nDiploid);
//            ZFZQF.setY(rect_BS.y()-20*nDiploid);
//            DrawQFRect(painter,ZFZQF, BSAN_QF, nElapsed,nDiploid,offset);
//        }

    }
    if(haveSGAN)
    {
        haveSGAN_QF = true;
        if(ButtonForm == 1)
        {
            Draw_Rect_Button(painter,//画笔
                             rect_SG,//按钮绘制区域//需要修改按钮区域大小为10
                             bFCZKState,//非常站控状态
                             getIsDown_SG(),//按钮是否按下
                             false,//封锁状态
                             false,//终端闪烁状态
                             0,//倒计时时间
                             THIN_GRAY,//按钮颜色
                             haveSGAN_QF,//铅封
                             SGAN_Name, //按钮文字显示
                             m_RangeVisible_BZDBS_SG,//按钮是否高亮
                             nElapsed,//闪烁读秒
                             nDiploid,//放大倍数
                             offset);//绘制KSK方形按钮
        }
        else
        {
            Draw_circle_Button(painter,//画笔
                               rect_SG,//按钮绘制区域//需要修改按钮区域大小为14
                               bFCZKState,//非常站控状态
                               getIsDown_SG(),//按钮是否按下
                               false,//封锁状态
                               false,//终端闪烁状态
                               0,//倒计时时间
                               haveSGAN_QF,//铅封
                               SGAN_Name, //按钮文字显示
                               m_RangeVisible_BZDBS_SG,//按钮是否高亮
                               nElapsed, //闪烁读秒
                               nDiploid, //放大倍数
                               offset);//绘制圆形按钮
        }
        //if(VISIBLE_JSQ)
        if((bShowQFJS)&&(haveSGAN_QF))
        {
            if(bVisleJSQ)
            {
                QPoint FCFZQF;
                FCFZQF.setX(rect_SG.x()-8*nDiploid);
                FCFZQF.setY(rect_SG.y()-20*nDiploid);
                DrawQFRect(painter,FCFZQF, SGAN_QF, nElapsed,nDiploid,offset);
            }
        }
    }
    if(haveFYAN)
    {
        haveFYAN_QF=false;
        if(ButtonForm == 1)
        {
            Draw_Rect_Button(painter,//画笔
                             rect_FY,//按钮绘制区域//需要修改按钮区域大小为10
                             bFCZKState,//非常站控状态
                             getIsDown_FY(),//按钮是否按下
                             false,//封锁状态
                             false,//终端闪烁状态
                             0,//倒计时时间
                             THIN_GRAY,//按钮颜色
                             haveFYAN_QF,//铅封
                             FYAN_Name, //按钮文字显示
                             m_RangeVisible_BZDBS_FY,//按钮是否高亮
                             nElapsed,//闪烁读秒
                             nDiploid,//放大倍数
                             offset);//绘制KSK方形按钮
        }
        else
        {
            Draw_circle_Button(painter,//画笔
                               rect_FY,//按钮绘制区域//需要修改按钮区域大小为14
                               bFCZKState,//非常站控状态
                               getIsDown_FY(),//按钮是否按下
                               false,//封锁状态
                               false,//终端闪烁状态
                               0,//倒计时时间
                               haveFYAN_QF,//铅封
                               FYAN_Name, //按钮文字显示
                               m_RangeVisible_BZDBS_FY,//按钮是否高亮
                               nElapsed, //闪烁读秒
                               nDiploid, //放大倍数
                               offset);//绘制圆形按钮
        }
        //if(VISIBLE_JSQ)
//        {//绘制接车辅助铅封
//            QPoint JCFZQF;
//            JCFZQF.setX(rect_FY.x()-4*nDiploid);
//            JCFZQF.setY(rect_FY.y()-20*nDiploid);
//            DrawQFRect(painter,JCFZQF, FYAN_QF, nElapsed,nDiploid,offset);
//        }
    }
    if(haveBSQHAN)
    {
        haveBSQHAN_QF=false;
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
                             haveBSQHAN_QF,//铅封
                             BSQHAN_Name, //按钮文字显示
                             m_RangeVisible_BZDBS_BSQH,//按钮是否高亮
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
                               haveBSQHAN_QF,//铅封
                               BSQHAN_Name, //按钮文字显示
                               m_RangeVisible_BZDBS_BSQH,//按钮是否高亮
                               nElapsed, //闪烁读秒
                               nDiploid, //放大倍数
                               offset);//绘制圆形按钮
        }
         if((bShowQFJS)&&(haveBSQHAN_QF))
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

    if(haveZDBSD)
    {
        Draw_Rect_Light(painter,//画笔
                        getBSD_ZDBSPt(),//指示灯中心坐标
                        ZDBSD_Name, //文字显示
                        getIsBSDLight_ZDBS(),
                        nElapsed,//闪烁读秒
                        nDiploid,//放大倍数
                        offset);//绘制指示灯
    }
    if(haveRGBSD)
    {
        Draw_Rect_Light(painter,//画笔
                        getBSD_RGBSPt(),//指示灯中心坐标
                        RGBSD_Name, //文字显示
                        getIsBSDLight_RGBS(),
                        nElapsed,//闪烁读秒
                        nDiploid,//放大倍数
                        offset);//绘制指示灯
    }
    if(haveKXD)
    {
        Draw_Rect_Light(painter,//画笔
                        getBSD_KXPt(),//指示灯中心坐标
                        KXD_Name, //文字显示
                        getIsBSDLight_KX(),
                        nElapsed,//闪烁读秒
                        nDiploid,//放大倍数
                        offset);//绘制指示灯
    }
    if(haveZYD)
    {
        Draw_Rect_Light(painter,//画笔
                        getBSD_ZYPt(),//指示灯中心坐标
                        ZYD_Name, //文字显示
                        getIsBSDLight_ZY(),
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
        QRect rec = fm.boundingRect(WordName);

        painter->setPen(QPen(Qt::black, 1));
        painter->setBrush(Qt::black);
        painter->drawRect(rect_Word);

        painter->setPen(Qt::white);//设置画笔颜色
        painter->drawText(rect_Word,Qt::AlignHCenter|Qt::AlignVCenter,WordName);

//        painter->setPen(QPen(Qt::black, 1));
//        painter->setBrush(Qt::black);
//        painter->drawRect(rec);

//        painter->setPen(Qt::white);//设置画笔颜色
//        painter->drawText(rec,Qt::AlignHCenter|Qt::AlignVCenter,WordName);
    }
}
void CBZDBS::setVollover(QPoint pt_Base)
{

}
unsigned int CBZDBS::getDevType()
{
    return Dev_BZDBS;
}

void CBZDBS::setGLXH(int x)
{
    m_nGLXH=x;
}
int CBZDBS::getGLXH()
{
    return m_nGLXH;
}
void CBZDBS::addVectJJQD(int x)
{
    vect_JJQD.append(x);
}
void CBZDBS::setVectJJQD(QVector<int> x)
{
    vect_JJQD=x;
}
QVector<int> CBZDBS::getVectJJQD()
{
    return vect_JJQD;
}
void CBZDBS::setIsNX(bool flag)
{
    isNX=flag;
}
bool CBZDBS::getIsNX()
{
    return isNX;
}
void CBZDBS::setArrowPt(QPoint pt)
{
    ArrowPt=pt;
    ArrowPtConst=pt;
}
QPoint CBZDBS::getArrowPt()
{
    return ArrowPtConst;
}
void CBZDBS::setAnNum(int x)
{
    m_nAnNum=x;
}
int CBZDBS::getAnNum()
{
    return m_nAnNum;
}
void CBZDBS::setBSQHANPt(QPoint pt)
{
    p_BSQHAN=pt;
    p_BSQHANConst=pt;
}
QPoint CBZDBS::getBSQHANPt()
{
    return p_BSQHANConst;
}
void CBZDBS::setBSANPt(QPoint pt)
{
    p_BSAN=pt;
    p_BSANConst=pt;
}
QPoint CBZDBS::getBSANPt()
{
    return p_BSANConst;
}
void CBZDBS::setSGANPt(QPoint pt)
{
    p_SGAN=pt;
    p_SGANConst=pt;
}
QPoint CBZDBS::getSGANPt()
{
    return p_SGANConst;
}
void CBZDBS::setFYANPt(QPoint pt)
{
    p_FYAN=pt;
    p_FYANConst=pt;
}
QPoint CBZDBS::getFYANPt()
{
    return p_FYANConst;
}
void CBZDBS::setMNPt(QPoint pt)
{
    p_MN=pt;
    p_MNConst=pt;
}
QPoint CBZDBS::getMNPt()
{
    return p_MNConst;
}
//闭塞切换
void CBZDBS::setIsDown_BSQH(bool flag)
{
    isDown_BSQH=flag;
}
bool CBZDBS::getIsDown_BSQH()
{
    return isDown_BSQH;
}
//闭塞
void CBZDBS::setIsDown_BS(bool flag)
{
    isDown_BS=flag;
}
bool CBZDBS::getIsDown_BS()
{
    return isDown_BS;
}
//事故
void CBZDBS::setIsDown_SG(bool flag)
{
    isDown_SG=flag;
}
bool CBZDBS::getIsDown_SG()
{
    return isDown_SG;
}
//复原
void CBZDBS::setIsDown_FY(bool flag)
{
    isDown_FY=flag;
}
bool CBZDBS::getIsDown_FY()
{
    return isDown_FY;
}
//void CBZDBS::setDownTime_FCFZ(int x)
//{
//    m_nDownTime_FCFZ=x;
//}
//int CBZDBS::getDownTime_FCFZ()
//{
//    return m_nDownTime_FCFZ;
//}
//void CBZDBS::setDownTime_JCFZ(int x)
//{
//    m_nDownTime_JCFZ=x;
//}
//int CBZDBS::getDownTime_JCFZ()
//{
//    return m_nDownTime_JCFZ;
//}
void CBZDBS::setBSD_ZYPt(QPoint pt)
{
    p_BSD_ZY=pt;
    p_BSD_ZYConst=pt;
}
QPoint CBZDBS::getBSD_ZYPt()
{
    return p_BSD_ZY;
}
void CBZDBS::setBSD_KXPt(QPoint pt)
{
    p_BSD_KX=pt;
    p_BSD_KXConst=pt;
}
QPoint CBZDBS::getBSD_KXPt()
{
    return p_BSD_KX;
}
void CBZDBS::setBSD_RGBSPt(QPoint pt)
{
    p_BSD_RGBS=pt;
    p_BSD_RGBSConst=pt;
}
QPoint CBZDBS::getBSD_RGBSPt()
{
    return p_BSD_RGBS;
}
void CBZDBS::setBSD_ZDBSPt(QPoint pt)
{
    p_BSD_ZDBS=pt;
    p_BSD_ZDBSConst=pt;
}
QPoint CBZDBS::getBSD_ZDBSPt()
{
    return p_BSD_ZDBS;
}
void CBZDBS::setRect_FK(QRect rect)
{
    rect_FK=rect;
    rect_FKConst=rect;
}
QRect CBZDBS::getRect_FK()
{
    return rect_FKConst;
}
void CBZDBS::setIsBSDLight_ZY(bool flag)
{
    isBSDLight_ZY=flag;
}
bool CBZDBS::getIsBSDLight_ZY()
{
    return isBSDLight_ZY;
}
void CBZDBS::setIsBSDLight_KX(bool flag)
{
    isBSDLight_KX=flag;
}
bool CBZDBS::getIsBSDLight_KX()
{
    return isBSDLight_KX;
}
void CBZDBS::setIsBSDLight_RGBS(bool flag)
{
    isBSDLight_RGBS=flag;
}
bool CBZDBS::getIsBSDLight_RGBS()
{
    return isBSDLight_RGBS;
}
void CBZDBS::setIsBSDLight_ZDBS(bool flag)
{
    isBSDLight_ZDBS=flag;
}
bool CBZDBS::getIsBSDLight_ZDBS()
{
    return isBSDLight_ZDBS;
}
void CBZDBS::setArrowStateJC(int x)
{
    m_nArrowStateJC=x;
}
int CBZDBS::getArrowStateJC()
{
    return m_nArrowStateJC;
}
void CBZDBS::setArrowStateFC(int x)
{
    m_nArrowStateFC=x;
}
int CBZDBS::getArrowStateFC()
{
    return m_nArrowStateFC;
}

void CBZDBS::DrawZDBSArrow(QPainter *painter,long nElapsed, double nDiploid,QPoint offset)
{
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);
    /*初始化箭头数据*/
    arrayArrowLeft[0] = QPoint((ArrowPt.x()+5*nDiploid), (ArrowPt.y()+32*nDiploid));
    arrayArrowLeft[1] = QPoint((ArrowPt.x()+30*nDiploid), (ArrowPt.y()+32*nDiploid));
    arrayArrowLeft[2] = QPoint((ArrowPt.x()+30*nDiploid), (ArrowPt.y()+24*nDiploid));
    arrayArrowLeft[3] = QPoint((ArrowPt.x()+50*nDiploid), (ArrowPt.y()+36*nDiploid));
    arrayArrowLeft[4] = QPoint((ArrowPt.x()+30*nDiploid), (ArrowPt.y()+48*nDiploid));
    arrayArrowLeft[5] = QPoint((ArrowPt.x()+30*nDiploid), (ArrowPt.y()+40*nDiploid));
    arrayArrowLeft[6] = QPoint((ArrowPt.x()+5*nDiploid), (ArrowPt.y()+40*nDiploid));
    arrayArrowLeft[7] = QPoint((ArrowPt.x()+5*nDiploid), (ArrowPt.y()+32*nDiploid));

    arrayArrowRight[0] = QPoint((ArrowPt.x()+125*nDiploid), (ArrowPt.y()+32*nDiploid));
    arrayArrowRight[1] = QPoint((ArrowPt.x()+100*nDiploid), (ArrowPt.y()+32*nDiploid));
    arrayArrowRight[2] = QPoint((ArrowPt.x()+100*nDiploid), (ArrowPt.y()+24*nDiploid));
    arrayArrowRight[3] = QPoint((ArrowPt.x()+80*nDiploid), (ArrowPt.y()+36*nDiploid));
    arrayArrowRight[4] = QPoint((ArrowPt.x()+100*nDiploid), (ArrowPt.y()+48*nDiploid));
    arrayArrowRight[5] = QPoint((ArrowPt.x()+100*nDiploid), (ArrowPt.y()+40*nDiploid));
    arrayArrowRight[6] = QPoint((ArrowPt.x()+125*nDiploid), (ArrowPt.y()+40*nDiploid));
    arrayArrowRight[7] = QPoint((ArrowPt.x()+125*nDiploid), (ArrowPt.y()+32*nDiploid));

    QPointF arrayArrowLeftForDraw[8];//左箭头
    QPointF arrayArrowRightForDraw[8];//右箭头
    for(int j = 0; j < 8; j++)
    {
        arrayArrowLeftForDraw[j] = QPoint(arrayArrowLeft[j].x(),
            arrayArrowLeft[j].y());
        arrayArrowRightForDraw[j] = QPoint(arrayArrowRight[j].x(),
            arrayArrowRight[j].y());
    }
    QPen pen;
    pen.setColor(GRAY);
    pen.setWidth(1);

    QBrush brush_Left;
    brush_Left.setStyle(Qt::SolidPattern);
    QColor color_Left;
    if(m_nArrowStateJC == Arrow_Red)
    {
        color_Left = RED;
    }
    else if(m_nArrowStateJC == Arrow_Yellow)
    {
        color_Left = YELLOW;
    }
    else if(m_nArrowStateJC == Arrow_Green)
    {
        color_Left = GREEN;
    }
    else if(m_nArrowStateJC == Arrow_Black)
    {
        color_Left = BLACK;
    }
    else if(m_nArrowStateJC == Arrow_White)
    {
        color_Left = WHITE;
    }
    else if(m_nArrowStateJC == Arrow_Gray)
    {
        color_Left = GREY;
    }

    QBrush brush_Right;
    brush_Right.setStyle(Qt::SolidPattern);
    QColor color_Right;
    if(m_nArrowStateFC == Arrow_Red)
    {
        color_Right = RED;
    }
    else if(m_nArrowStateFC == Arrow_Yellow)
    {
        color_Right = YELLOW;
    }
    else if(m_nArrowStateFC == Arrow_Green)
    {
        color_Right = GREEN;
    }
    else if(m_nArrowStateFC == Arrow_Black)
    {
        color_Right = BLACK;
    }
    else if(m_nArrowStateFC == Arrow_White)
    {
        color_Right = WHITE;
    }
    else if(m_nArrowStateFC == Arrow_Gray)
    {
        color_Right = GREY;
    }
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
    if(m_nSX)//0是下行,1是上行
    {
        if(bStaSXLORR)
        {
            brush_Left.setColor(color_Right);
            painter->setPen(pen);
            painter->setBrush(brush_Left);
            painter->drawConvexPolygon(arrayArrowLeftForDraw, 8);

            brush_Right.setColor(color_Left);
            painter->setPen(pen);
            painter->setBrush(brush_Right);
            painter->drawConvexPolygon(arrayArrowRightForDraw, 8);
        }
        else
        {
            brush_Left.setColor(color_Left);
            painter->setPen(pen);
            painter->setBrush(brush_Left);
            painter->drawConvexPolygon(arrayArrowLeftForDraw, 8);

            brush_Right.setColor(color_Right);
            painter->setPen(pen);
            painter->setBrush(brush_Right);
            painter->drawConvexPolygon(arrayArrowRightForDraw, 8);
        }
    }
    else
    {
        if(bStaSXLORR)
        {
            brush_Left.setColor(color_Left);
            painter->setPen(pen);
            painter->setBrush(brush_Left);
            painter->drawConvexPolygon(arrayArrowLeftForDraw, 8);

            brush_Right.setColor(color_Right);
            painter->setPen(pen);
            painter->setBrush(brush_Right);
            painter->drawConvexPolygon(arrayArrowRightForDraw, 8);
        }
        else
        {
            brush_Left.setColor(color_Right);
            painter->setPen(pen);
            painter->setBrush(brush_Left);
            painter->drawConvexPolygon(arrayArrowLeftForDraw, 8);

            brush_Right.setColor(color_Left);
            painter->setPen(pen);
            painter->setBrush(brush_Right);
            painter->drawConvexPolygon(arrayArrowRightForDraw, 8);
        }

    }

}

void CBZDBS::DrawQFRect(QPainter *painter,QPointF QFPoint, int m_nQFNumber, long nElapsed, double nDiploid,QPoint offset)
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

void CBZDBS::Draw_circle_Button(QPainter *painter,//画笔
                              QRectF m_ButtonRect,//按钮绘制区域
                              bool m_FCZKState,//非常站控状态
                              int m_ButtonIsDownState,//按钮是否按下
                              bool m_FSState,//封锁状态
                              bool m_bZDANFlash,//终端闪烁状态
                              int mTimeCount,//倒计时时间
                              bool HaveQF,//是否有铅封计数
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
//    if (0<m_ButtonIsDownState && m_ButtonIsDownState<4)
//    {
//        if (nElapsed % 2 == 0)
//        {
//            btnClrTopLeft = WHITE;//按钮边框-TL
//            btnClrBottomRight = GRAY;//按钮边框-BR
//        }
//        else
//        {
//            btnClrTopLeft = GRAY;//按钮边框-TL
//            btnClrBottomRight = WHITE;//按钮边框-BR
//        }
//    }
    if (m_ButtonIsDownState==1)
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
    if (m_ButtonIsDownState==2)
    {
        btnClrTopLeft = GRAY;//按钮边框-TL
        btnClrBottomRight = WHITE;//按钮边框-BR
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
//    if(m_FCZKState)//非常站控模式下
    if(0)//非常站控模式下
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
                    newBrushBK.setColor(THIN_GRAY);
                    painter->setBrush(newBrushBK);

                    pen.setColor(THIN_GRAY);
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
    if(rectTextShow != "")
    {
        if(HaveQF)
        {
            pen.setColor(YELLOW);
        }
        else
        {
            pen.setColor(WHITE);
        }
        QFont font;
        font.setWeight(50);//字体宽度,0~99
        font.setPointSize(12 * nDiploid);//字体高度,10表示高度为10个像素点
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

void CBZDBS::Draw_Rect_Button(QPainter *painter,//画笔
                              QRectF m_ButtonRect,//按钮绘制区域
                              bool m_FCZKState,//非常站控状态
                              int m_ButtonIsDownState,//按钮是否按下
                              bool m_FSState,//封锁状态
                              bool m_bZDANFlash,//终端闪烁状态
                              int mTimeCount,//倒计时时间
                              QColor color,//按钮颜色
                              bool HaveQF,//是否有铅封计数
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
//    if (0<m_ButtonIsDownState && m_ButtonIsDownState<4)
//    {
//        if (nElapsed % 2 == 0)
//        {
//            btnClrTopLeft = WHITE;//按钮边框-TL
//            btnClrBottomRight = GRAY;//按钮边框-BR
//        }
//        else
//        {
//            btnClrTopLeft = GRAY;//按钮边框-TL
//            btnClrBottomRight = WHITE;//按钮边框-BR
//        }
//    }
//    if(!m_FCZKState)
    if(1)
    {
        if (m_ButtonIsDownState==1)
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
    if(0)
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
                    newBrushBK.setColor(color);
                    painter->setBrush(newBrushBK);

                    pen.setColor(color);
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
        if(HaveQF)
        {
            pen.setColor(YELLOW);
        }
        else
        {
            pen.setColor(WHITE);
        }

        QFont font;
        font.setWeight(50);//字体宽度,0~99
        //font.setPointSize(12 * nDiploid);//字体高度,10表示高度为10个像素点
        font.setPointSize(BSWordSize * nDiploid);//字体高度,10表示高度为10个像素点
        painter->setPen(pen);
        painter->setFont(font);
        QRectF rectBtnWord_X;
        rectBtnWord_X.setLeft(m_ButtonRect.left()-30 * nDiploid);
        rectBtnWord_X.setTop(m_ButtonRect.top()+15 * nDiploid);
        rectBtnWord_X.setRight(m_ButtonRect.right()+30 * nDiploid);
        rectBtnWord_X.setBottom(m_ButtonRect.bottom()+25 * nDiploid);
//        painter->drawText(rectBtnTG_X,Qt::AlignHCenter|Qt::AlignVCenter,m_strName + "TA");
       // if(bBtnNameShow)
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

void CBZDBS::Draw_Rect_Light(QPainter *painter,//画笔
                            QPointF LightPoint,//指示灯中心坐标
                            QString rectTextShow, //文字显示
                            bool Light_color,//指示灯状态
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
        color_Light = getLightState(2,nElapsed);
    }
    else
    {
        color_Light = THIN_GRAY;
    }
    newBrushBK.setColor(color_Light);
    painter->setBrush(newBrushBK);

    QPen pen;
    pen.setColor(THIN_GRAY);
    painter->setPen(pen);

    CenterPoint = QPointF(LightPoint.x(),LightPoint.y());
    painter->drawEllipse(CenterPoint, 8 * nDiploid, 8 * nDiploid);

    //绘制指示灯的文字
    if(rectTextShow != "")
    {
        pen.setColor(WHITE);
        QFont font;
        font.setWeight(50);//字体宽度,0~99
        font.setPointSize(BSWordSize * nDiploid);//字体高度,10表示高度为10个像素点
        painter->setPen(pen);
        painter->setFont(font);
        QRectF rectBtnWord_X;
        rectBtnWord_X.setLeft(LightPoint.x()-30 * nDiploid);
        rectBtnWord_X.setTop(LightPoint.y()+10 * nDiploid);
        rectBtnWord_X.setRight(LightPoint.x()+30 * nDiploid);
        rectBtnWord_X.setBottom(LightPoint.y()+30 * nDiploid);
        painter->drawText(rectBtnWord_X,Qt::AlignHCenter|Qt::AlignVCenter,rectTextShow);
    }
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, false);
#pragma endregion
}

QColor CBZDBS::getLightState(int nType,int nElapsed)//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝）
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

void CBZDBS::SetANIsDown(int nType)
{
    QString strName="";
    if(nType==1)//闭塞
    {
        setIsDown_BS(1);
        strName="闭塞";
    }
    else
    {
        setIsDown_BS(0);
    }
    if(nType==2)//事故
    {
        setIsDown_SG(1);
         strName="事故";
    }
    else
    {
        setIsDown_SG(0);
    }
    if(nType==3)//复原
    {
       setIsDown_FY(1);
        strName="事故";
    }
    else
    {
        setIsDown_FY(0);
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
QString CBZDBS::GetANIsDown(int nType)
{
    QString strName="";
    if(nType==1)//闭塞
    {
        strName="闭塞";
    }
    if(nType==2)//事故
    {
         strName="事故";
    }
    if(nType==3)//复原
    {
        strName="复原";
    }
    if(nType==4)//闭塞切换按钮
    {
         strName="闭塞切换";
    }
    return strName;
}
bool CBZDBS::GetQFByAnType(int nytpe)
{
    bool bqf=false;
    //   bool haveBSQHAN_QF;//闭塞切换铅封标志
//    bool haveBSAN_QF;//闭塞按钮铅封标志
//    bool haveSGAN_QF;//事故按钮铅封标志
//    bool haveFYAN_QF;//复原按钮铅封标志
    if(nytpe==1)//闭塞
    {
        bqf=haveBSAN_QF;
    }
    if(nytpe==2)//事故
    {
        bqf=haveSGAN_QF;
    }
    if(nytpe==3)//复原
    {
        bqf=haveFYAN_QF;
    }
    if(nytpe==4)//闭塞切换按钮
    {
        bqf=haveBSQHAN_QF;
    }
    return bqf;
}

bool CBZDBS::GetMousePoint_BSQH(QPoint p)
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
bool CBZDBS::GetMousePoint_BS(QPoint p)
{
    if (rect_BS.contains(p))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool CBZDBS::GetMousePoint_SG(QPoint p)
{
    if (rect_SG.contains(p))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool CBZDBS::GetMousePoint_FY(QPoint p)
{
    if (rect_FY.contains(p))
    {
        return true;
    }
    else
    {
        return false;
    }
}










