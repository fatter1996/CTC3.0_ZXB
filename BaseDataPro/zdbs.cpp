#include "zdbs.h"

CZDBS::CZDBS()
{
    m_nGLXH=0xffff;
    isNX=0;
    ArrowPt=QPoint(0,0);
    ArrowPtConst=QPoint(0,0);
    m_nAnNum=0;
    p_GFAN=QPoint(0,0);
    p_GFANConst=QPoint(0,0);
    p_ZFZAN=QPoint(0,0);
    p_ZFZANConst=QPoint(0,0);
    p_FCFZAN=QPoint(0,0);
    p_FCFZANConst=QPoint(0,0);
    p_JCFZAN=QPoint(0,0);
    p_JCFZANConst=QPoint(0,0);
    p_MN=QPoint(0,0);
    p_MNConst=QPoint(0,0);
    rect_GF=QRect(0,0,0,0);
    rect_ZFZ=QRect(0,0,0,0);
    rect_FCFZ=QRect(0,0,0,0);
    rect_JCFZ=QRect(0,0,0,0);
    isDown_GF=0;
    isDown_ZFZ=0;
    isDown_FCFZ=0;
    isDown_JCFZ=0;
    m_nDownTime_FCFZ=0xFF;
    m_nDownTime_JCFZ=0xFF;
    p_BSD_YXFC=QPoint(0,0); //允许发车灯
    p_BSD_YXFCConst=QPoint(0,0); //允许发车灯
    p_BSD_FZ=QPoint(0,0);  //辅助表示灯
    p_BSD_FZConst=QPoint(0,0);  //辅助表示灯
    p_BSD_SG=QPoint(0,0);  //闪光表示灯
    p_BSD_SGConst=QPoint(0,0);  //闪光表示灯
    p_BSD_QJ=QPoint(0,0);  //区间表示灯
    p_BSD_QJConst=QPoint(0,0);  //区间表示灯
    rect_FK=QRect(0,0,0,0);
    rect_FKConst=QRect(0,0,0,0);
    isBSDLight_YXFC=false;
    isBSDLight_SG=false;
    isBSDLight_FZ=false;
    isBSDLight_QJ=false;
    m_nArrowStateJC=Arrow_Black;
    m_nArrowStateFC=Arrow_Black;

    GFAN_Name = "";
    ZFZAN_Name = "";
    FCFZAN_Name = "";
    JCFZAN_Name = "";
    YXFC_Name = "";
    FZ_Name = "";
    SG_Name = "";
    QJ_Name = "";
    ArrowLeftName = "";
    ArrowRightName = "";

    haveZFZAN = true;
    haveFCFZAN = true;
    haveJCFZAN = true;
    haveYXFC = true;
    haveFZ = true;
    haveSG = true;
    haveQJ = true;

    GFAN_QF = 0;
    ZFZAN_QF = 0;
    FCFZAN_QF = 0;
    JCFZAN_QF = 0;

    haveGFAN_QF = false;
    haveZFZAN_QF = false;
    haveFCFZAN_QF = false;
    haveJCFZAN_QF = false;
    haveZDBS_QF = false;

    haveFK = false;
    ptFK=QPoint(0,0);
    FK_length = 0;
    FK_breath = 0;

    haveWord = false;
    ptWord = QPoint(0,0);
    WordName = "";

    m_RangeVisible = false;
    ButtonForm = 1;

    QJDColorType = 1;
    SGDColorType = 1;
    FZDColorType = 1;
    YXFCDColorType = 1;

    m_nDownTime_ZFZ_1 = 0;   //当按钮按下为1时持续时间
    m_nDownTime_JCFZ_1 = 0;
    m_nDownTime_FCFZ_1 = 0;
    m_nDownTime_GF_1 = 0;

    m_RangeVisible_ZDBS_ZFZ = false; //设置设备范围显示（或高亮显示）//点灯灭灯按下
    m_RangeVisible_ZDBS_JCFZ = false; //设置设备范围显示（或高亮显示）//点灯灭灯按下
    m_RangeVisible_ZDBS_FCFZ = false; //设置设备范围显示（或高亮显示）//点灯灭灯按下
    m_RangeVisible_ZDBS_GF = false; //设置设备范围显示（或高亮显示）//点灯灭灯按下
}
void CZDBS::Draw_ToolTip(QPainter *painter, double nDiploid)
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
int CZDBS::moveCursor(QPoint p)
{
    if (rect_ZFZ.contains(p))
    {
        return 1;
    }
    else if(rect_FCFZ.contains(p))
    {
        return 2;
    }
    else if(rect_JCFZ.contains(p))
    {
        return 3;
    }
    else if (rect_GF.contains(p))
    {
        return 4;
    }
    return 0;
}
void CZDBS::Draw(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type)
{
    if(type==0x55)
    {
        Draw_Th_kb(painter,nElapsed, nDiploid,offset, false);
    }
}
void CZDBS::DrawM(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type,int flag)
{
    if(type==0x55)
    {
        Draw_Th_kb(painter,nElapsed, nDiploid,offset, true);
    }
}

void CZDBS::Draw_Th_kb(QPainter *painter,long nElapsed, double nDiploid,QPoint offset, bool isMulti)
{
    QFont font;
    ArrowPt.setX(ArrowPtConst.x() * nDiploid+offset.x()*nDiploid);
    ArrowPt.setY(ArrowPtConst.y() * nDiploid+offset.y()*nDiploid);

    if(ButtonForm == 1)
    {
        rect_GF.setRect(p_GFANConst.x() * nDiploid + offset.x() * nDiploid,p_GFANConst.y() * nDiploid + offset.y() * nDiploid,10*nDiploid,10*nDiploid);
        rect_ZFZ.setRect(p_ZFZANConst.x() * nDiploid + offset.x() * nDiploid,p_ZFZANConst.y() * nDiploid + offset.y() * nDiploid,10*nDiploid,10*nDiploid);
        rect_FCFZ.setRect(p_FCFZANConst.x() * nDiploid + offset.x() * nDiploid,p_FCFZANConst.y() * nDiploid + offset.y() * nDiploid,10*nDiploid,10*nDiploid);
        rect_JCFZ.setRect(p_JCFZANConst.x() * nDiploid + offset.x() * nDiploid,p_JCFZANConst.y() * nDiploid + offset.y() * nDiploid,10*nDiploid,10*nDiploid);
    }
    else
    {
        rect_GF.setRect(p_GFANConst.x() * nDiploid + offset.x() * nDiploid,p_GFANConst.y() * nDiploid + offset.y() * nDiploid,14*nDiploid,14*nDiploid);
        rect_ZFZ.setRect(p_ZFZANConst.x() * nDiploid + offset.x() * nDiploid,p_ZFZANConst.y() * nDiploid + offset.y() * nDiploid,14*nDiploid,14*nDiploid);
        rect_FCFZ.setRect(p_FCFZANConst.x() * nDiploid + offset.x() * nDiploid,p_FCFZANConst.y() * nDiploid + offset.y() * nDiploid,14*nDiploid,14*nDiploid);
        rect_JCFZ.setRect(p_JCFZANConst.x() * nDiploid + offset.x() * nDiploid,p_JCFZANConst.y() * nDiploid + offset.y() * nDiploid,14*nDiploid,14*nDiploid);
    }

    p_BSD_SG.setX(p_BSD_SGConst.x()  * nDiploid+offset.x()*nDiploid);
    p_BSD_SG.setY(p_BSD_SGConst.y()  * nDiploid+offset.y()*nDiploid);
    p_BSD_FZ.setX(p_BSD_FZConst.x()  * nDiploid+offset.x()*nDiploid);
    p_BSD_FZ.setY(p_BSD_FZConst.y()  * nDiploid+offset.y()*nDiploid);
    p_BSD_QJ.setX(p_BSD_QJConst.x()  * nDiploid+offset.x()*nDiploid);
    p_BSD_QJ.setY(p_BSD_QJConst.y()  * nDiploid+offset.y()*nDiploid);
    p_BSD_YXFC.setX(p_BSD_YXFCConst.x()  * nDiploid+offset.x()*nDiploid);
    p_BSD_YXFC.setY(p_BSD_YXFCConst.y()  * nDiploid+offset.y()*nDiploid);

//    rect_FK.setRect(rect_FKConst.topLeft().x() * nDiploid + offset.x() * nDiploid,rect_FKConst.topLeft().y() * nDiploid + offset.y() * nDiploid,rect_FKConst.width()*nDiploid,rect_FKConst.height()*nDiploid);
    rect_FK.setRect(ptFK.x() * nDiploid + offset.x() * nDiploid,ptFK.y() * nDiploid + offset.y() * nDiploid,FK_length*nDiploid,FK_breath*nDiploid);
    int WordNameLenth;
    WordNameLenth = WordName.length();
    rect_Word.setRect(ptWord.x() * nDiploid + offset.x() * nDiploid,ptWord.y() * nDiploid + offset.y() * nDiploid,WordNameLenth*12*nDiploid,20*nDiploid);
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);

    font.setFamily("宋体");
    font.setPointSize(10 * nDiploid);//字号
    font.setItalic(false);//斜体
    painter->setFont(font);//设置字体
    painter->setPen(Qt::white);//设置画笔颜色

    QString strNameAN[4]={"总辅助","发车辅助","接车辅助","改方按钮"};
    int isDownAN[4]={0,0,0,0};
    isDownAN[0]=getIsDown_ZFZ();
    isDownAN[1]=getIsDown_FCFZ();
    isDownAN[2]=getIsDown_JCFZ();
    isDownAN[3]=getIsDown_GF();
    QRect rectAN[4]={rect_ZFZ,rect_FCFZ,rect_JCFZ,rect_GF};

    QString strNameBSD[4]={"允许发车","辅助","闪光","区间"};
    int isLight[4]={0,0,0,0};
    isLight[0]=getIsBSDLight_YXFC();
    isLight[1]=getIsBSDLight_FZ();
    isLight[2]=getIsBSDLight_SG();
    isLight[3]=getIsBSDLight_QJ();
    QPoint bsdPt[4]={p_BSD_YXFC,p_BSD_FZ,p_BSD_SG,p_BSD_QJ};

    DrawZDBSArrow(painter,nElapsed,nDiploid,offset);//绘制箭头
    if(isMulti)
        return;
    if(/*VISIBLE_JSQ*/0)
    {//绘制自动闭塞铅封
        QPoint ArrowQF;
        ArrowQF.setX(ArrowPt.x()+56*nDiploid);
        ArrowQF.setY(ArrowPt.y()+28*nDiploid);
        DrawQFRect(painter,ArrowQF, ZDBS_QF, nElapsed,nDiploid,offset);
    }
    if(haveZFZAN)
    {
        haveZFZAN_QF = true;
        if(ButtonForm == 1)
        {
            Draw_Rect_Button(painter,//画笔
                             rect_ZFZ,//按钮绘制区域//需要修改按钮区域大小为10
                             bFCZKState,//非常站控状态
                             getIsDown_ZFZ(),//按钮是否按下
                             false,//封锁状态
                             false,//终端闪烁状态
                             0,//倒计时时间
                             THIN_GRAY,//按钮颜色
                             haveZFZAN_QF,
                             ZFZAN_Name, //按钮文字显示
                             m_RangeVisible_ZDBS_ZFZ,//按钮是否高亮
                             nElapsed,//闪烁读秒
                             nDiploid,//放大倍数
                             offset);//绘制KSK方形按钮
        }
        else
        {
            Draw_circle_Button(painter,//画笔
                               rect_ZFZ,//按钮绘制区域//需要修改按钮区域大小为14
                               bFCZKState,//非常站控状态
                               getIsDown_ZFZ(),//按钮是否按下
                               false,//封锁状态
                               false,//终端闪烁状态
                               0,//倒计时时间
                               haveZFZAN_QF,
                               ZFZAN_Name, //按钮文字显示
                               m_RangeVisible_ZDBS_ZFZ,//按钮是否高亮
                               nElapsed, //闪烁读秒
                               nDiploid, //放大倍数
                               offset);//绘制圆形按钮
        }
        if((bShowQFJS)&&(haveZFZAN_QF))
        {//绘制总辅助铅封
            if(bVisleJSQ)
            {
                QPoint ZFZQF;
                ZFZQF.setX(rect_ZFZ.x()-8*nDiploid);
                ZFZQF.setY(rect_ZFZ.y()-20*nDiploid);
                DrawQFRect(painter,ZFZQF, ZFZAN_QF, nElapsed,nDiploid,offset);
            }
        }
    }
    if(haveFCFZAN)
    {
        haveFCFZAN_QF = true;
        if(ButtonForm == 1)//方形按钮
        {
            Draw_Rect_Button(painter,//画笔
                             rect_FCFZ,//按钮绘制区域//需要修改按钮区域大小为10
                             bFCZKState,//非常站控状态
                             getIsDown_FCFZ(),//按钮是否按下
                             false,//封锁状态
                             false,//终端闪烁状态
                             0,//倒计时时间
                             THIN_GRAY,//按钮颜色
                             haveFCFZAN_QF,
                             FCFZAN_Name, //按钮文字显示
                             m_RangeVisible_ZDBS_FCFZ,//按钮是否高亮
                             nElapsed,//闪烁读秒
                             nDiploid,//放大倍数
                             offset);//绘制KSK方形按钮
        }
        else//圆形按钮
        {
            Draw_circle_Button(painter,//画笔
                               rect_FCFZ,//按钮绘制区域//需要修改按钮区域大小为14
                               bFCZKState,//非常站控状态
                               getIsDown_FCFZ(),//按钮是否按下
                               false,//封锁状态
                               false,//终端闪烁状态
                               0,//倒计时时间
                               haveFCFZAN_QF,
                               FCFZAN_Name, //按钮文字显示
                               m_RangeVisible_ZDBS_FCFZ,//按钮是否高亮
                               nElapsed, //闪烁读秒
                               nDiploid, //放大倍数
                               offset);//绘制圆形按钮
        }
        if((bShowQFJS)&&(haveFCFZAN_QF))
        {//绘制发车辅助铅封
            if(bVisleJSQ)
            {
                QPoint FCFZQF;
                FCFZQF.setX(rect_FCFZ.x()-8*nDiploid);
                FCFZQF.setY(rect_FCFZ.y()-20*nDiploid);
                DrawQFRect(painter,FCFZQF, FCFZAN_QF, nElapsed,nDiploid,offset);
            }
        }
    }
    if(haveJCFZAN)
    {
        haveJCFZAN_QF = true;
        if(ButtonForm == 1)
        {
            Draw_Rect_Button(painter,//画笔
                             rect_JCFZ,//按钮绘制区域//需要修改按钮区域大小为10
                             bFCZKState,//非常站控状态
                             getIsDown_JCFZ(),//按钮是否按下
                             false,//封锁状态
                             false,//终端闪烁状态
                             0,//倒计时时间
                             THIN_GRAY,//按钮颜色
                             haveJCFZAN_QF,
                             JCFZAN_Name, //按钮文字显示
                             m_RangeVisible_ZDBS_JCFZ,//按钮是否高亮
                             nElapsed,//闪烁读秒
                             nDiploid,//放大倍数
                             offset);//绘制KSK方形按钮
        }
        else
        {
            Draw_circle_Button(painter,//画笔
                               rect_JCFZ,//按钮绘制区域//需要修改按钮区域大小为14
                               bFCZKState,//非常站控状态
                               getIsDown_JCFZ(),//按钮是否按下
                               false,//封锁状态
                               false,//终端闪烁状态
                               0,//倒计时时间
                               haveJCFZAN_QF,
                               JCFZAN_Name, //按钮文字显示
                               m_RangeVisible_ZDBS_JCFZ,//按钮是否高亮
                               nElapsed, //闪烁读秒
                               nDiploid, //放大倍数
                               offset);//绘制圆形按钮
        }
        if((bShowQFJS)&&(haveJCFZAN_QF))
        {//绘制接车辅助铅封
            if(bVisleJSQ)
            {
                QPoint JCFZQF;
                JCFZQF.setX(rect_JCFZ.x()-8*nDiploid);
                JCFZQF.setY(rect_JCFZ.y()-20*nDiploid);
                DrawQFRect(painter,JCFZQF, JCFZAN_QF, nElapsed,nDiploid,offset);
            }
        }
    }
    if(isNX)//是否有改方
    {
        haveGFAN_QF = true;
        if(ButtonForm == 1)
        {
            Draw_Rect_Button(painter,//画笔
                             rect_GF,//按钮绘制区域//需要修改按钮区域大小为10
                             bFCZKState,//非常站控状态
                             getIsDown_GF(),//按钮是否按下
                             false,//封锁状态
                             false,//终端闪烁状态
                             0,//倒计时时间
                             THIN_GRAY,//按钮颜色
                             haveGFAN_QF,
                             GFAN_Name, //按钮文字显示
                             m_RangeVisible_ZDBS_GF,//按钮是否高亮
                             nElapsed,//闪烁读秒
                             nDiploid,//放大倍数
                             offset);//绘制KSK方形按钮
        }
        else
        {
            Draw_circle_Button(painter,//画笔
                               rect_GF,//按钮绘制区域//需要修改按钮区域大小为14
                               bFCZKState,//非常站控状态
                               getIsDown_GF(),//按钮是否按下
                               false,//封锁状态
                               false,//终端闪烁状态
                               0,//倒计时时间
                               haveGFAN_QF,
                               GFAN_Name, //按钮文字显示
                               m_RangeVisible_ZDBS_GF,//按钮是否高亮
                               nElapsed, //闪烁读秒
                               nDiploid, //放大倍数
                               offset);//绘制圆形按钮
        }
        if((bShowQFJS)&&(haveGFAN_QF))
        {//绘制改方铅封
            if(bVisleJSQ)
            {
                QPoint GFQF;
                GFQF.setX(rect_GF.x()-8*nDiploid);
                GFQF.setY(rect_GF.y()-20*nDiploid);
                DrawQFRect(painter,GFQF, GFAN_QF, nElapsed,nDiploid,offset);
            }
        }
    }

    if(haveQJ)
    {
        Draw_circle_Light(painter,//画笔
                        getBSD_QJPt(),//指示灯中心坐标
                        QJ_Name, //文字显示
                        getIsBSDLight_QJ(),
                        QJDColorType,
                        nElapsed,//闪烁读秒
                        nDiploid,//放大倍数
                        offset);//绘制指示灯
    }
    if(haveFZ)
    {
        Draw_circle_Light(painter,//画笔
                        getBSD_FZPt(),//指示灯中心坐标
                        FZ_Name, //文字显示
                        getIsBSDLight_FZ(),
                        FZDColorType,
                        nElapsed,//闪烁读秒
                        nDiploid,//放大倍数
                        offset);//绘制指示灯
    }
    if(haveSG)
    {
        Draw_circle_Light(painter,//画笔
                        getBSD_SGPt(),//指示灯中心坐标
                        SG_Name, //文字显示
                        getIsBSDLight_SG(),
                        SGDColorType,
                        nElapsed,//闪烁读秒
                        nDiploid,//放大倍数
                        offset);//绘制指示灯
    }
    if(haveYXFC)
    {
        Draw_circle_Light(painter,//画笔
                        getBSD_YXFCPt(),//指示灯中心坐标
                        YXFC_Name, //文字显示
                        getIsBSDLight_YXFC(),
                        YXFCDColorType,
                        nElapsed,//闪烁读秒
                        nDiploid,//放大倍数
                        offset);//绘制指示灯
    }


//    //绘制按钮
//    for(int i=0;i<getAnNum();i++)
//    {
//        if(i>=4)
//        {
//            break;
//        }
//        if(isDownAN[i] == true)
//        {
//            draw_Pixmap(painter,":/Pictures/IDB_AN_DOWN_Yellow.jpg",rectAN[i].topLeft().x(),rectAN[i].topLeft().y(),14,14,nDiploid);
//        }
//        else
//        {
//            draw_Pixmap(painter,":/Pictures/IDB_AN_UP_White.jpg",rectAN[i].topLeft().x(),rectAN[i].topLeft().y(),14,14,nDiploid);
//        }
//        QFontMetrics fm(font);
//        QRect rec = fm.boundingRect(strNameAN[i]);
//        painter->drawText(rectAN[i].topLeft().x()+6* nDiploid-rec.width()/2, rectAN[i].topLeft().y()+30* nDiploid, strNameAN[i]);
//    }
//    //绘制表示灯
//    for(int i=0;i<4;i++)
//    {
//        if(isLight[i] == true)
//        {
//            draw_Pixmap(painter,":/Pictures/LED_Red.jpg",bsdPt[i].x(),bsdPt[i].y(),13,13,nDiploid);
//        }
//        else
//        {
//            draw_Pixmap(painter,":/Pictures/LED_Black.jpg",bsdPt[i].x(),bsdPt[i].y(),13,13,nDiploid);
//        }
//        QFontMetrics fm(font);
//        QRect rec = fm.boundingRect(strNameBSD[i]);
//        painter->drawText(bsdPt[i].x()+5* nDiploid-rec.width()/2, bsdPt[i].y()+30* nDiploid, strNameBSD[i]);
//    }
//    //绘制箭头
//    if(getSX() == 0)  //接车箭头在左，发车箭头在右
//    {
//        if(getArrowStateJC() == Arrow_Black)
//        {
//            draw_Pixmap(painter,":/Pictures/ARightGrayA.jpg",ArrowPt.x()-15,ArrowPt.y(),20,15,nDiploid);
//        }
//        else if(getArrowStateJC() == Arrow_Red)
//        {
//            draw_Pixmap(painter,":/Pictures/ALeftRed.jpg",ArrowPt.x()-15,ArrowPt.y(),20,15,nDiploid);
//        }
//        else if(getArrowStateJC() == Arrow_Green)
//        {
//            draw_Pixmap(painter,":/Pictures/ARightGreen.jpg",ArrowPt.x()-15,ArrowPt.y(),20,15,nDiploid);
//        }
//        else if(getArrowStateJC() == Arrow_Yellow)
//        {
//            draw_Pixmap(painter,":/Pictures/ARightYellow.jpg",ArrowPt.x()-15,ArrowPt.y(),20,15,nDiploid);
//        }

//        if(getArrowStateFC() == Arrow_Black)
//        {
//            draw_Pixmap(painter,":/Pictures/ALeftGrayA.jpg",ArrowPt.x()+15,ArrowPt.y(),20,15,nDiploid);
//        }
//        else if(getArrowStateFC() == Arrow_Red)
//        {
//            draw_Pixmap(painter,":/Pictures/ALeftRed.jpg",ArrowPt.x()+15,ArrowPt.y(),20,15,nDiploid);
//        }
//        else if(getArrowStateFC() == Arrow_Green)
//        {
//            draw_Pixmap(painter,":/Pictures/ALeftGreen.jpg",ArrowPt.x()+15,ArrowPt.y(),20,15,nDiploid);
//        }
//        else if(getArrowStateFC() == Arrow_Yellow)
//        {
//            draw_Pixmap(painter,":/Pictures/ALeftYellow.jpg",ArrowPt.x()+15,ArrowPt.y(),20,15,nDiploid);
//        }
//    }
//    else
//    {
//        if(getArrowStateFC() == Arrow_Black)
//        {
//            draw_Pixmap(painter,":/Pictures/ARightGrayA.jpg",ArrowPt.x()-15,ArrowPt.y(),20,15,nDiploid);
//        }
//        else if(getArrowStateFC() == Arrow_Red)
//        {
//            draw_Pixmap(painter,":/Pictures/ARightRed.jpg",ArrowPt.x()-15,ArrowPt.y(),20,15,nDiploid);
//        }
//        else if(getArrowStateFC() == Arrow_Green)
//        {
//            draw_Pixmap(painter,":/Pictures/ARightGreen.jpg",ArrowPt.x()-15,ArrowPt.y(),20,15,nDiploid);
//        }
//        else if(getArrowStateFC() == Arrow_Yellow)
//        {
//            draw_Pixmap(painter,":/Pictures/ARightYellow.jpg",ArrowPt.x()-15,ArrowPt.y(),20,15,nDiploid);
//        }

//        if(getArrowStateJC() == Arrow_Black)
//        {
//            draw_Pixmap(painter,":/Pictures/ALeftGrayA.jpg",ArrowPt.x()+15,ArrowPt.y(),20,15,nDiploid);
//        }
//        else if(getArrowStateJC() == Arrow_Red)
//        {
//            draw_Pixmap(painter,":/Pictures/ALeftRed.jpg",ArrowPt.x()+15,ArrowPt.y(),20,15,nDiploid);
//        }
//        else if(getArrowStateJC() == Arrow_Green)
//        {
//            draw_Pixmap(painter,":/Pictures/ALeftGreen.jpg",ArrowPt.x()+15,ArrowPt.y(),20,15,nDiploid);
//        }
//        else if(getArrowStateJC() == Arrow_Yellow)
//        {
//            draw_Pixmap(painter,":/Pictures/ALeftYellow.jpg",ArrowPt.x()+15,ArrowPt.y(),20,15,nDiploid);
//        }
//    }

    //绘制方框
    if(haveFK)
    {
        painter->setPen(QPen(Qt::white, 1));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect_FK);
    }
    //绘制文字 //方向文字显示标志
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
void CZDBS::setVollover(QPoint pt_Base)
{

}
unsigned int CZDBS::getDevType()
{
    return Dev_ZDBS;
}

void CZDBS::setGLXH(int x)
{
    m_nGLXH=x;
}
int CZDBS::getGLXH()
{
    return m_nGLXH;
}
void CZDBS::addVectJJQD(int x)
{
    vect_JJQD.append(x);
}
void CZDBS::setVectJJQD(QVector<int> x)
{
    vect_JJQD=x;
}
QVector<int> CZDBS::getVectJJQD()
{
    return vect_JJQD;
}
void CZDBS::setIsNX(bool flag)
{
    isNX=flag;
}
bool CZDBS::getIsNX()
{
    return isNX;
}
void CZDBS::setArrowPt(QPoint pt)
{
    ArrowPt=pt;
    ArrowPtConst=pt;
}
QPoint CZDBS::getArrowPt()
{
    return ArrowPtConst;
}
void CZDBS::setAnNum(int x)
{
    m_nAnNum=x;
}
int CZDBS::getAnNum()
{
    return m_nAnNum;
}
void CZDBS::setGFANPt(QPoint pt)
{
    p_GFAN=pt;
    p_GFANConst=pt;
}
QPoint CZDBS::getGFANPt()
{
    return p_GFANConst;
}
void CZDBS::setZFZANPt(QPoint pt)
{
    p_ZFZAN=pt;
    p_ZFZANConst=pt;
}
QPoint CZDBS::getZFZANPt()
{
    return p_ZFZANConst;
}
void CZDBS::setFCFZANPt(QPoint pt)
{
    p_FCFZAN=pt;
    p_FCFZANConst=pt;
}
QPoint CZDBS::getFCFZANPt()
{
    return p_FCFZANConst;
}
void CZDBS::setJCFZANPt(QPoint pt)
{
    p_JCFZAN=pt;
    p_JCFZANConst=pt;
}
QPoint CZDBS::getJCFZANPt()
{
    return p_JCFZANConst;
}
void CZDBS::setMNPt(QPoint pt)
{
    p_MN=pt;
    p_MNConst=pt;
}
QPoint CZDBS::getMNPt()
{
    return p_MNConst;
}
//改方
void CZDBS::setIsDown_GF(int flag)
{
    isDown_GF=flag;
}
int CZDBS::getIsDown_GF()
{
    return isDown_GF;
}
//总辅助
void CZDBS::setIsDown_ZFZ(int flag)
{
    isDown_ZFZ=flag;
}
int CZDBS::getIsDown_ZFZ()
{
    return isDown_ZFZ;
}
//发车辅助
void CZDBS::setIsDown_FCFZ(int flag)
{
    isDown_FCFZ=flag;
}
int CZDBS::getIsDown_FCFZ()
{
    return isDown_FCFZ;
}
//接车辅助
void CZDBS::setIsDown_JCFZ(int flag)
{
    isDown_JCFZ=flag;
}
int CZDBS::getIsDown_JCFZ()
{
    return isDown_JCFZ;
}
//发车辅助按下时间
void CZDBS::setDownTime_FCFZ(int x)
{
    m_nDownTime_FCFZ=x;
}
int CZDBS::getDownTime_FCFZ()
{
    return m_nDownTime_FCFZ;
}
//接车辅助按下时间
void CZDBS::setDownTime_JCFZ(int x)
{
    m_nDownTime_JCFZ=x;
}
int CZDBS::getDownTime_JCFZ()
{
    return m_nDownTime_JCFZ;
}
void CZDBS::setBSD_YXFCPt(QPoint pt)
{
    p_BSD_YXFC=pt;
    p_BSD_YXFCConst=pt;
}
QPoint CZDBS::getBSD_YXFCPt()
{
    return p_BSD_YXFC;
}
void CZDBS::setBSD_SGPt(QPoint pt)
{
    p_BSD_SG=pt;
    p_BSD_SGConst=pt;
}
QPoint CZDBS::getBSD_SGPt()
{
    return p_BSD_SG;
}
void CZDBS::setBSD_FZPt(QPoint pt)
{
    p_BSD_FZ=pt;
    p_BSD_FZConst=pt;
}
QPoint CZDBS::getBSD_FZPt()
{
    return p_BSD_FZ;
}
void CZDBS::setBSD_QJPt(QPoint pt)
{
    p_BSD_QJ=pt;
    p_BSD_QJConst=pt;
}
QPoint CZDBS::getBSD_QJPt()
{
    return p_BSD_QJ;
}
void CZDBS::setRect_FK(QRect rect)
{
    rect_FK=rect;
    rect_FKConst=rect;
}
QRect CZDBS::getRect_FK()
{
    return rect_FKConst;
}
void CZDBS::setIsBSDLight_YXFC(int flag)
{
    isBSDLight_YXFC=flag;
}
int CZDBS::getIsBSDLight_YXFC()
{
    return isBSDLight_YXFC;
}
void CZDBS::setIsBSDLight_SG(int flag)
{
    isBSDLight_SG=flag;
}
int CZDBS::getIsBSDLight_SG()
{
    return isBSDLight_SG;
}
void CZDBS::setIsBSDLight_FZ(int flag)
{
    isBSDLight_FZ=flag;
}
int CZDBS::getIsBSDLight_FZ()
{
    return isBSDLight_FZ;
}
void CZDBS::setIsBSDLight_QJ(int flag)
{
    isBSDLight_QJ=flag;
}
int CZDBS::getIsBSDLight_QJ()
{
    return isBSDLight_QJ;
}
void CZDBS::setArrowStateJC(int x)
{
    m_nArrowStateJC=x;
}
int CZDBS::getArrowStateJC()
{
    return m_nArrowStateJC;
}
void CZDBS::setArrowStateFC(int x)
{
    m_nArrowStateFC=x;
}
int CZDBS::getArrowStateFC()
{
    return m_nArrowStateFC;
}
void CZDBS::DrawZDBSArrow(QPainter *painter,long nElapsed, double nDiploid,QPoint offset)
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
//    if(m_nSX)//0是下行,1是上行
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
//    else
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

void CZDBS::DrawQFRect(QPainter *painter,QPointF QFPoint, int m_nQFNumber, long nElapsed, double nDiploid,QPoint offset)
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

void CZDBS::Draw_circle_Button(QPainter *painter,//画笔
                              QRectF m_ButtonRect,//按钮绘制区域
                              bool m_FCZKState,//非常站控状态
                              int m_ButtonIsDownState,//按钮是否按下
                              bool m_FSState,//封锁状态
                              bool m_bZDANFlash,//终端闪烁状态
                              int mTimeCount,//倒计时时间
                              bool HaveQF,//是否有铅封
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
    if(m_FCZKState)//非常站控模式下
    {
        if(m_ButtonIsDownState == 2)
        {
            QBrush newBrushBK;
            newBrushBK.setStyle(Qt::SolidPattern);
            newBrushBK.setColor(RED);
            painter->setBrush(newBrushBK);

            pen.setColor(RED);
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

void CZDBS::Draw_Rect_Button(QPainter *painter,//画笔
                              QRectF m_ButtonRect,//按钮绘制区域
                              bool m_FCZKState,//非常站控状态
                              int m_ButtonIsDownState,//按钮是否按下
                              bool m_FSState,//封锁状态
                              bool m_bZDANFlash,//终端闪烁状态
                              int mTimeCount,//倒计时时间
                              QColor color,//按钮颜色
                              bool HaveQF,//是否有铅封
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
    else if(0)//按钮封锁-粉闪
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
                        newBrushBK.setColor(THIN_GRAY);
                        painter->setBrush(newBrushBK);

                        pen.setColor(THIN_GRAY);
                        painter->setPen(pen);
                    }
                }
                else
                {
                    newBrushBK.setColor(THIN_GRAY);
                    painter->setBrush(newBrushBK);

                    pen.setColor(THIN_GRAY);
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

void CZDBS::Draw_circle_Light(QPainter *painter,//画笔
                              QPointF LightPoint,//指示灯中心坐标
                              QString rectTextShow, //文字显示
                              bool Light_color,//指示灯状态
                              int colorType,//颜色类型
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
    if(Light_color)//点灯状态
    {
        color_Light = getLightState(colorType,nElapsed);
    }
    else//灭灯状态
    {
        color_Light = BLACK;
    }
    newBrushBK.setColor(color_Light);
    painter->setBrush(newBrushBK);

    QPen pen;
    pen.setWidth(2);
    pen.setColor(THIN_GRAY);
    painter->setPen(pen);

    CenterPoint = QPointF(LightPoint.x(),LightPoint.y());
    painter->drawEllipse(CenterPoint, 8 * nDiploid, 8 * nDiploid);

    //绘制指示灯的文字
    if(rectTextShow != ""&&bShowhave)
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

QColor CZDBS::getLightState(int nType,int nElapsed)//（颜色：0灭灯,1红,2绿,3白,4黄,5红闪,6绿闪,7白闪,8黄闪,9蓝）
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
//设置高亮状态
void CZDBS::SetRangeVisible(bool b)
{
    m_RangeVisible=b;
}
//获取高亮状态
bool CZDBS::GetRangeVisible()
{
    return m_RangeVisible;
}
void CZDBS::SetANIsDown(int nType)
{
    QString strName="";
    if(nType==1)//总辅助
    {
        setIsDown_ZFZ(1);
        strName="总辅助";
    }
    else
    {
        //setIsDown_ZFZ(0);
    }
    if(nType==2)//发车辅助
    {
        setIsDown_FCFZ(1);
        strName="发车辅助";
    }
    else
    {
        //setIsDown_FCFZ(0);
    }
    if(nType==3)//接车辅助
    {
        setIsDown_JCFZ(1);
        strName="接车辅助";
    }  else
    {
        //setIsDown_JCFZ(0);
    }
    if(nType==4)//改方
    {
        setIsDown_GF(1);
        strName="改方";
    }
    else
    {
        //setIsDown_GF(0);
    }
    //return strName;
}
QString CZDBS::GetANIsDown(int nType)
{
    QString strName="";
    if(nType==1)//总辅助
    {
        //strName="总辅助";
        strName=ZFZAN_Name;
    }
    if(nType==2)//发车辅助
    {
        //strName="发车辅助";
        strName=FCFZAN_Name;
    }
    if(nType==3)//接车辅助
    {
        //strName="接车辅助";
        strName=JCFZAN_Name;
    }
    if(nType==4)//改方
    {
        //strName="允许改方";
        strName=GFAN_Name;
    }
    return strName;
}

bool CZDBS::GetMousePoint_ZFZ(QPoint p)
{
    if (rect_ZFZ.contains(p))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool CZDBS::GetMousePoint_JCFZ(QPoint p)
{
    if (rect_JCFZ.contains(p))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool CZDBS::GetMousePoint_FCFZ(QPoint p)
{
    if (rect_FCFZ.contains(p))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool CZDBS::GetMousePoint_GF(QPoint p)
{
    if (rect_GF.contains(p))
    {
        return true;
    }
    else
    {
        return false;
    }
}










