#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtMath>
#include <QObject>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QDir>
#include <QtDebug>
#include <QTimer>
#include <QDateTime>
#include <QSettings>
#include <QSignalMapper>
#include <QtSql/QSqlDatabase>
#include <QStandardItemModel>
#include <QLabel>

#pragma execution_character_set("utf-8")

//软件版本号
//#define VERSION  "CTC车务终端_V2.0.1.0"
#define COMPANY  "兰州安信铁路科技有限公司"
#define SYSTITLE "CTC-车务终端仿真培训系统 V2.0 值班员终端 A机 "

//方向
#define LEFT      0x5A
#define RIGHT     0xA5

//吸起落下
#define UP        0x55
#define DOWN      0xAA

//客户区坐标偏移量
#define Off_Global 0

//颜色定义
#define SkyBlue    QColor(85,120,182)
#define GREY       QColor(128,128,128)
#define GDBK_GRAY  QColor(34, 36 ,62)
/*颜色定义*/
#define WHITE      QColor(253, 253, 253)
//#define WHITEX     RGB(192, 192, 192)
#define WHITEX     QColor(250, 250, 250)
#define BLUE       QColor(0, 0, 255) //RGB(17, 17, 255)
#define LIGHT_BLUEX QColor(88, 88, 255) //引导浅蓝色
#define LIGHT_BLUE QColor(80, 190, 255) //浅蓝色
#define THIN_BLUE_DOWN QColor(0, 229, 238)
#define XHDZ_BLUE  QColor(65, 115, 170)
#define THIN_BLUE  QColor(65, 115, 170)
//#define BLUEKX     RGB(27, 88, 231)
#define BLUEKX     QColor(65, 115, 170)
#define GREEN      QColor(12, 255, 12)
#define THIN_GREEN QColor(67, 205, 128)
#define BROWN      QColor(205, 41, 144)	//用于封锁功能显示色

#define DEFAULTCOLOR QColor(65, 115, 225)
#define AGREEN     QColor(69, 139, 116)
#define DEEP_GREEN QColor(28, 168, 38)
#define LIGHT_GREEN QColor(111, 220, 104) //RGB(171, 240, 159)
#define YELLOW     QColor(255, 255, 0)
#define LIGHT_YELLOW QColor(239, 228, 176)//淡黄色
#define BLACK      QColor(0, 0, 0)
#define RED        QColor(255, 12, 12)
#define LIGHTRED   QColor(255, 134, 134)
//#define LIGHTBULE   QColor(255, 134, 134)
#define LIGHTBULE   QColor(0, 192, 192)
#define THIN_WHITE QColor(120, 120, 120)
#define DEEP_GRAY  QColor(72, 72, 72)
#define GRAY       QColor(120, 120, 120)
//#define THIN_GRAY  QColor(192, 192 ,192)
#define THIN_GRAY  QColor(180, 180 ,180)
//#define THIN_THIN_GRAY  QColor(212, 212 ,212)
#define THIN_THIN_GRAY  QColor(205, 205 ,205)
#define PINK       QColor(240, 32, 209)    // casco界面粉红色（分路不良颜色方式1）
#define FLBL_COLOR QColor(255, 255, 255) //白色 （分路不良颜色方式2）
#define DARKSEAGREEN2  QColor(180, 238, 180)	//
#define ZHANTAICOLOR1 QColor(172, 60, 172) //粉红色 高站台颜色
#define ZHANTAICOLOR2 QColor(0, 179, 0)//粉红色 低站台颜色
#define PURPLE     QColor(213, 30, 218)//粉红色 行车日志字体颜色
#define XCRZ_BKCOLOR QColor(0xFF, 0xFF, 0xFF) //行车日志背景色 淡黄色 RGB(0xFF, 0xFF, 0xE0)

//区段状态定义
#define GD_WHITE   0x0001
#define GD_BLUE    0x0002
#define GD_GREEN   0x0003
#define GD_RED     0x0004

#define GD_QD      0x000A  //股道区段
#define JJ_QD      0x000B  //接近区段 区间
#define QD         0x000C  //无岔区段

//信号机相关定义
#define JZ_XHJ     0x0100
#define DCJL_XHJ   0x0101
#define JZFS_XHJ   0x0102
#define SXCZ_XHJ   0x0103
#define YG_XHJ     0x0104
#define CZ_XHJ     0x0105
#define DC_XHJ     0x0106
#define DCFS_XHJ   0x0107
#define FCJL_XHJ   0x0108
#define JLFS_XHJ   0x0109
#define XHP_XHJ    0x010A

//站台线条宽度
#define ZHANTAIWIDTH  3
#define ZHANTAIHEIGHT 15 //站台高度
//#define XHD_DS     0x0110
//#define XHD_HD     0x0111
//#define XHD_AD     0x0112
//#define XHD_BD     0x0113
//#define XHD_LD     0x0114
//#define XHD_UD     0x0115
//#define XHD_UU     0x0116
//#define XHD_LL     0x0117
//#define XHD_YD     0x0118
//#define XHD_LU     0x0119
//#define XHD_2U     0x011A
//#define XHD_2L     0x011B
//#define XHD_BS     0x011C
//#define XHD_US     0x011D
//#define XHD_LS     0x011E
//#define XHD_USU    0x011F
//#define XHD_MD     0x0120
//#define XHD_YB     0x0121

//信号灯状态--CTC
#define XHD_DS      0x0000     //信号灯断丝
#define XHD_BD      0x0001     //信号灯白灯
#define XHD_AD      0x0002     //信号灯兰灯
#define XHD_HD      0x0004     //信号灯红灯
#define XHD_LD      0x0008     //信号灯绿灯
#define XHD_UD      0x0010     //信号灯单黄
#define XHD_UU      0x0020     //信号灯双黄
#define XHD_LL      0x0040     //信号灯双绿
#define XHD_YD      0x0080     //信号灯引导（红白）
#define XHD_LU      0x0100     //信号灯绿黄
#define XHD_2U      0x0200     //信号灯二黄
#define XHD_BS      0x0400     //信号灯白闪
#define XHD_HS      0x0800     //信号灯红闪
#define XHD_US      0x1000     //信号灯黄闪
#define XHD_LS      0x2000     //信号灯绿闪
#define XHD_USU     0x4000     //信号灯黄闪黄
#define XHD_MD      0x8000     //信号灯灭

#define LCAN       0x0126
#define DCAN       0x0127
#define YDAN       0x0128
#define TGAN       0x0129
#define GDDCAN     0x012A
#define QDAN       0x012B
#define ZDBSAN     0x012C
#define BZDBSAN    0x012D

#define ClearAllGZ 0x012F

#define XH_CZXH    0x0130
#define XH_DZXH    0x0131
#define XH_BZXH    0x0132
#define XH_JTXXH   0x0133

//设备类型相关定义
#define Dev_XH    0x0150
#define Dev_DC    0x0151
#define Dev_GD    0x0152
#define Dev_QD    0x0153
#define Dev_JTX   0x0154
#define Dev_TEXT  0x0155
#define Dev_YDQ   0x0156
#define Dev_DCQD  0x0157
#define Dev_WCQD  0x0158
#define Dev_TG    0x0159
#define Dev_ZDBS  0x015A
#define Dev_BZDBS 0x015B
#define Dev_JZ    0x015C
#define Dev_LAMP  0x015D
#define Dev_FUNCB  0x015E
#define Dev_ZT    0x015F
#define Dev_DCBTN 0x0160
#define Dev_ATWIN 0x0161
#define Dev_CL 0x0162

#define DCGZ_JG 0x01  //道岔故障按钮-尖轨
#define DCGZ_XG 0x02  //道岔故障按钮-心轨
#define TEXT_DCGZAN TEXT("道岔故障按钮")
#define CHECIRECT_WIDTH 15
#define CHECIRECT_LENGTH 110
////菜单相关定义
//#define QDGZ     0x1601
//#define QDZY     0x1602
//#define QDFLBL   0x1603
//#define DCCQFLBL 0x1604
//#define DCDWFLBL 0x1605
//#define DCFWFLBL 0x1606
//#define DCWZ     0x1607
//#define YDQGZ    0x1608
//#define LEUGZ    0x1609
//#define ZCRESET  0x160A

////轨道道岔相关定义
#define GDWIDTH    4
#define GDWIDTH2    5
#define JYJ12      8            //岔前绝缘节
#define JYJ34      16           //定位绝缘节
#define JYJ56      32           //反位绝缘节
//#define DCDW       0x0200       //定位
//#define DCFW       0x0201       //反位
//#define DCSK       0x0202       //四开

//闪烁
#define XHDBTN_FLASTIME 2		//4个单位长
#define XHD_FLASTIME 2			//4个单位长

//文本相关定义
#define STATIONNAMETEXT   0x0300
#define DCQDNAMETEXT      0x0301
#define TEXT              0x0302

//应答器相关定义
#define YDQ_Q            0x0400
#define YDQ_FQ           0x0401
#define YDQ_DW           0x0402
#define YDQ_JZ           0x0403
#define YDQ_CZ           0x0404
#define YDQ_FCZ          0x0405
#define YDQ_FJZ          0x0406
#define YDQ_DC           0x0407
#define YDQ_DD           0x0408
#define YDQ_ZJ           0x0409
#define YDQ_RL           0x040A
#define YDQ_ZX_R         0x040B
#define YDQ_YG_R         0x040C
#define YDQ_ZC_3_2       0x040D
#define YDQ_YG_3_2       0x040E

#define YDQ_WY     0x0420
#define YDQ_YY     0x0421

//低频码相关定义
#define DMH_H    0x0500
#define DMH_HB   0x0501
#define DMH_HU   0x0502
#define DMH_UU   0x0503
#define DMH_UUS  0x0504   //双黄闪码
#define DMH_U2   0x0505   //黄2码
#define DMH_U2S  0x0506   //黄2闪码
#define DMH_U    0x0507
#define DMH_LU   0x0508
#define DMH_L    0x0509
#define DMH_L2   0x050A
#define DMH_L3   0x050B
#define DMH_L4   0x050C
#define DMH_L5   0x050D
#define DMH_JC   0x050E
#define DMH_SP   0x050F  //机车信号设备的载频锁定或切换
#define DMH_WM   0x0510

//功能按钮功能代码
#define Fun_FCZK        0x0600
#define Fun_XYDZS       0x0601
#define Fun_SYDZS       0x0602
#define Fun_ZQX         0x0603
#define Fun_ZRJ         0x0604
#define Fun_QGJ         0x0605
#define Fun_DCZD        0x0606
#define Fun_DCZF        0x0607
#define Fun_CLEAR       0x0608
#define Fun_DCDS        0x0609
#define Fun_DCDJ        0x060A
#define Fun_ANFS        0x060B
#define Fun_ANJF        0x060C
#define Fun_DCFS        0x060D
#define Fun_DCJF        0x060E
#define Fun_DD          0x060F
#define Fun_MD          0x0610
#define Fun_SDJS        0x0611
#define Fun_FZMENU      0x0612
#define Fun_MARK        0x0613
#define Fun_FLBL        0x0614
#define Fun_QRWCZY      0x0615
#define Fun_QJZJS       0x0616
#define Fun_OPENLJJC    0x0617
#define Fun_CLOSELJJC   0x0618
#define Fun_PDJS        0x0619
#define Fun_CQFLBL      0x061A
#define Fun_DWFLBL      0x061B
#define Fun_FWFLBL      0x061C
#define Fun_XHANDOWN    0x061D
#define Fun_TGANDOWN    0x061E
#define Fun_GDFS        0x061F
#define Fun_GDJF        0x0620
//#define Fun_FCZK        0x0621
#define Fun_GFAN        0x0622
#define Fun_ZFZ         0x0623
#define Fun_FCFZ        0x0624
#define Fun_JCFZ        0x0625

#define Arrow_Red       0x0701
#define Arrow_Yellow    0x0702
#define Arrow_Green     0x0703
#define Arrow_Black     0x0704
#define Arrow_White     0x0705
#define Arrow_Gray      0x0706

//******************* CTC服务终端 *******************
//CTC制式
//软件类型或数据类型Byte[8]
#define DATATYPE_LS    0xAA //联锁
#define DATATYPE_TCC   0xBB //教师机
#define DATATYPE_CTC   0xCC //CTC
#define DATATYPE_CTRL  0xCA //集控
#define DATATYPE_BOARD 0xCD //占线板
#define DATATYPE_TRAIN 0xAB //培训软件

//接发车计划类型
#define JFC_TYPE_JF 0xAA //正常(接发)
#define JFC_TYPE_SF 0xBB //始发
#define JFC_TYPE_ZD 0xCC //终到
#define JFC_TYPE_TG 0xDD //通过

//软件类型或数据类型Byte[8]
#define DATATYPE_LS    0xAA //联锁
#define DATATYPE_TCC   0xBB //教师机
#define DATATYPE_CTC   0xCC //CTC
#define DATATYPE_CTRL  0xCA //集控
#define DATATYPE_SERVER 0xDD //服务 DATATYPE_CTRL
#define DATATYPE_BOARD 0xCD //占线板
#define DATATYPE_TRAIN 0xAB //培训软件
//功能类型Byte[9]
#define FUNCTYPE_STAGEPLNEW 0x33 //阶段计划（CTC人工新增）
#define FUNCTYPE_DISPTCH 0x34 //调度命令
#define FUNCTYPE_STAGEPL 0x35 //阶段计划签收
#define FUNCTYPE_TRAFFICLOG 0x35 //行车日志操作
#define FUNCTYPE_ROUTE   0x36 //进路序列
#define FUNCTYPE_TRAFFIC 0x37 //行车日志
#define FUNCTYPE_SECTION 0x38 //区间逻辑检查
#define FUNCTYPE_FLOWS   0x39 //作业流程办理
#define FUNCTYPE_CHGMODE 0x3A //控制模式转换
#define FUNCTYPE_BTNCLICK 0x3C //进路按钮按下（终端闪烁）
#define FUNCTYPE_DEVOPERA 0x3D //设备操作
#define FUNCTYPE_CMDCLEAR 0x3E //命令清除
#define FUNCTYPE_USERLOGINEXIT 0x3F //用户登录注销操作
#define FUNCTYPE_SPEACH   0x42 //（Server->CTC）语音播报
//数据同步消息定义-TCP协议
#define FUNCTYPE_SYNC    0x3B //同步类型
#define FUNCTYPE_CHECK   0x43 //（Server<->CTC）防错办进路检查
//功能按钮
#define FUNCTYPE_FUNCS   0x88
#define FUNCTYPE_DDMLQS  0x04 //（CTC->联锁）调度命令签收
#define FUNCTYPE_XCRZ    0x05 //（CTC->联锁）行车日志操作
#define FUNCTYPE_MDYCC   0xAC //（CTC->联锁）车次操作 0x44 车次号修正
#define FUNCTYPE_DDMLZF  0x07 //（CTC->联锁）调度命令转发司机
#define FUNCTYPE_CHGCC   0x61 //车次计划修改（行车日志）
#define FUNCTYPE_LIMITCC 0x67 //（CTC->联锁）车次限速
//数据更新同步命令-Byte[9]-有数据变化，需要各终端重新读取数据库进行同步
#define FUNCTYPE_UPDATE  0x3B
//更新数据类型(子分类码)
#define UPDATETYPE_ALL  0x00 //全部
#define UPDATETYPE_JDJH 0x01 //阶段计划
#define UPDATETYPE_XCRZ 0x02 //行车日志
#define UPDATETYPE_JLXL 0x03 //进路序列
#define UPDATETYPE_GDFL 0x04 //股道防溜
#define UPDATETYPE_DDML 0x05 //调度命令
#define UPDATETYPE_KZMS 0x06 //控制模式
#define UPDATETYPE_JLQX 0x07 //进路权限
//同步数据类型(子分类码)-TCP协议
#define SYNC_ALL  0xAA //全部
#define SYNC_JDJH 0x01 //阶段计划
#define SYNC_XCRZ 0x02 //行车日志
#define SYNC_JLXL 0x03 //进路序列
#define SYNC_GDFL 0x04 //股道防溜
#define SYNC_DDML 0x05 //调度命令
#define SYNC_FCB  0x06 //防错办基础数据
//同步数据标志(子分类码)-TCP协议
#define SYNC_FLAG_REQUEST 0xAA //请求
#define SYNC_FLAG_ADD     0x11 //增加
#define SYNC_FLAG_DELETE  0x22 //删除
#define SYNC_FLAG_UPDATE  0x33 //更新
#define SYNC_FLAG_DELALL  0x44 //删除所有

//计划和进路的控制命令(和占线板通信协议保持一致)
#define PLAN_CMD_TYPE 0x51 //分类码-车次计划执行命令
#define PLAN_CMD_CHG  0x61 //分类码-车次计划修改
#define PLAN_CMD_SIGN 0x62 //分类码-车次计划签收
#define PLAN_CMD_SFQX 0x91 //分类码-进路权限释放命令
#define PLAN_CMD_FUNC 0x88 //分类码-功能命令
 //计划和进路的控制子命令(和占线板通信协议保持一致)
#define PLAN_CMD_FCYG 0x01 //发车预告
#define PLAN_CMD_TYYG 0x02 //同意预告
#define PLAN_CMD_DDBD 0x03 //到达报点
#define PLAN_CMD_CFBD 0x04 //出发报点
#define PLAN_CMD_TGBD 0x05 //通过报点
#define PLAN_CMD_LZCF 0x06 //邻站出发
#define PLAN_CMD_JCJL 0x07 //接车进路
#define PLAN_CMD_FCJL 0x08 //发车进路
#define PLAN_CMD_QXJL 0x09 //取消接车进路
#define PLAN_CMD_QXFL 0x0A //取消发车进路
#define PLAN_CMD_QXJY 0x0B //取消接车预告
#define PLAN_CMD_QXFY 0x0C //取消发车预告
#define PLAN_CMD_TGJL 0x0D //通过进路
#define PLAN_CMD_CHGJCBTJL 0x0E //修改接车变通进路
#define PLAN_CMD_CHGFCBTJL 0x0F //修改发车变通进路
#define PLAN_CMD_JCBTJL 0x10 //修改并办理接车变通进路
#define PLAN_CMD_FCBTJL 0x11 //修改并办理接车变通进路

//Time格式化为CString
#define TIME_FORMAT_YMDHMSM "yyyy-MM-dd hh:mm:ss.zzz"
#define TIME_FORMAT_YMDHMS  "yyyy-MM-dd hh:mm:ss"
#define TIME_FORMAT_YMDHM   "yyyy-MM-dd hh:mm"
#define TIME_FORMAT_YMD     "yyyy-MM-dd"
#define TIME_FORMAT_HMS     "hh:mm:ss"
#define TIME_FORMAT_HM      "hh:mm"

#define CLEANQD 0x0FF00      //用以清除区段状态
#define CLEANDC 0x0F0FF      //用以清除道岔状态

//列车类型
#define LCTYPE_KC 0x01 //客车
#define LCTYPE_HC 0x00 //货车

#define ROUTE_JC 0x00  //接车进路
#define ROUTE_FC 0x01  //发车进路

//超限等级
#define CHAOXIAN_0 "正常"
#define CHAOXIAN_1 "一级超限"
#define CHAOXIAN_2 "二级超限"
#define CHAOXIAN_3 "三级超限"
#define CHAOXIAN_4 "超级超限"

#define CTCCENTER "CTC调度中心"

#define ROUTE_JC 0x00  //接车进路
#define ROUTE_FC 0x01  //发车进路
//#define ROUTE_TG 0x03  //通过进路
#define ROUTE_TONGG 0x03  //通过进路

//列车位置信息状态
#define STATUS_LCWJJ   "列车未接近"
#define STATUS_LC3FQ   "列车距离本站3个分区"
#define STATUS_LC2FQ   "列车距离本站2个分区"
#define STATUS_LC1FQ   "列车距离本站1个分区"
#define STATUS_LCYJZ   "列车进站" //列车已进站
#define STATUS_LCYKSJRGD   "列车开始进入股道"
#define STATUS_LCYWZJRGD "列车完整进入股道" //列车已进入股道
#define STATUS_LCYCZ   "列车已出站"
#define STATUS_LCYWC   "列车已完成"

//列车办理流程提示信息
#define PROCESS_JCYG "办理接车预告"
#define PROCESS_BLJL  "办理接车进路"
#define PROCESS_LCDD  "列车到达(通过)报点"
#define PROCESS_FCYG "办理发车预告"
#define PROCESS_BLFL  "办理发车进路"
#define PROCESS_LCCF  "发车报点"
#define PROCESS_FINISH "流程停止"

#define ZHCXC  0x01  //正常行车
#define LCMN   0x02  //列车模拟
#define DCMN   0x03  //调车模拟

//进路办理状态
#define STATUS_JLBL_NO   0x00 //未办理
#define STATUS_JLBL_ING  0x01  //正在办理
#define STATUS_JLBL_SUCC 0x02  //办理成功

/*指示灯类型*/
#ifndef LAMP_TYPE_H
#define	CTC_MODE_CENTER		0x0001	//中心模式
#define	CTC_MODE_STATION	0x0002	//车站控制模式
#define	CTC_MODE_NORMAL		0x0003	//分散自律模式
#define CTC_MODE_6502		0x0004	//非常站控
#define BUILDROUTE_BYPLAN	0x0005	//按图排路
#define CONTROL_BYPLAN		0x0006	//计划控制
#define COMM_TOCENTER		0x0007	//中心通信
#define COMM_TOINTERLOCK	0x0008	//自律机通信
#define PERMIT_BACK			0x0009	//允许转回
#define TRAFFIC_CONTROL		0x000A	//列控
#define AGREE     0xAA //同意
#define DISAGREE  0xAB //不同意
#endif

//作业流程定义
#define DEF_FLOW_JL 0x01
#define DEF_FLOW_LJ 0x02
#define DEF_FLOW_SS 0x03
#define DEF_FLOW_XW 0x04
#define DEF_FLOW_JP 0x05
#define DEF_FLOW_CJ 0x06
#define DEF_FLOW_ZG 0x07
#define DEF_FLOW_LW 0x08
#define DEF_FLOW_HJ 0x09
#define DEF_FLOW_HC 0x0A
#define DEF_FLOW_ZX 0x0B
#define DEF_FLOW_JC 0x0C
#define DEF_FLOW_DK 0x0D
#define DEF_FLOW_CH 0x0E
#define DEF_FLOW_ZK 0x0F
#define DEF_FLOW_ZW 0x10

//作业按钮状态
#define BTNSTATUS_WAP  0x00 //未安排(灰色)、未办理
#define BTNSTATUS_YAP  0x01 //已安排(红色)、需要办理
#define BTNSTATUS_ZBL  0x02 //正在办理(黄色)得知
#define BTNSTATUS_YBL  0x03 //已办理(绿色)

#define  FrameHead   0xEF     //通信数据帧的帧头
#define  FrameTail   0xFE     //通信数据帧的帧尾

//区段状态定义
#define QDGZ    0x0010       //区段故障
#define QDKX    0x0001       //区段空闲
#define QDSB    0x0002       //区段锁闭
#define QDZY    0x0004       //区段占用
#define QDYSB   0x0008       //区段预锁闭
//道岔状态
#define DCDW    0x0100       //定位
#define DCFW    0x0200       //反位
#define DCSK    0x0400       //四开

#define CLEANQD 0x0FF00      //用以清除区段状态
#define CLEANDC 0x0F0FF      //用以清除道岔状态

//列车调车进路命令2字节高两位设备按钮类型定义.lwm.2021.6.22
#define JLBTN_LC   0x00   //进路按钮-列车按钮 0000 0000
#define JLBTN_DC   0x40   //进路按钮-调车按钮 0100 0000
#define JLBTN_TG   0x80   //进路按钮-调车按钮 1000 0000
#define MASK05     0x3F   //用以得到字节的0~5位

#define LC_ROUTE    0x5A          //列车进路
#define DC_ROUTE    0xA5          //调车进路
#define TG_ROUTE    0x55          //通过进路
#define ROUTE_LC    "LCJL"   //列车进路
#define ROUTE_DC    "DCJL"   //调车进路
#define ROUTE_TG    "TGJL"   //通过进路

#define COMMUNICATION_TIPS "未检测到有效的服务端和联锁通信，无法操作！"
#define COMMUNICATION_LOST_MSG "系统服务端或联锁通信中断！"

#define LCJL_AUTOTOUCH_MIN 1 //列车进路自触时间->分钟
#define LCJL_AUTOTOUCH_SEC 60 //列车进路自触时间->秒

////接车进路自动触发提前时间（分钟）
//extern int AutoTouchReachRouteLeadMinutes;
////发车进路自动触发提前时间（分钟）
//extern int AutoTouchDepartRouteLeadMinutes;

//关于股道显示状态
#define TEXT_SHOW       0x01    // 文字显示
#define GD_JYJ_SHOW     0x02	//绝缘节显示:|0x02
#define GD_CHECIWND_SHOW	0x04	//车次窗显示:|0x04
#define GDDC_X_LOCK     0x08//X总锁闭
#define GDDC_S_LOCK     0x10//S总锁闭
#define FCZK_MODE       0x20 //非常站控 用于按钮显示灰色
#define XHD_LCTGBTN_SHOW 0x40 //列车通过按钮显示
#define XHD_DCBTN_SHOW  0x80	//调车按钮显示
#define GD_JIEJINWND_SHOW 0x0100 //接近窗显示
#define GD_TEXT_SHOW	0x0200	// 股道 文字显示:|0x01
#define DC_TEXT_SHOW	0x0400	// 道岔 文字显示:|0x01
#define ZM_TEXT_SHOW    0x0800    // 站名
#define NML_TEXT_SHOW   0x1000    // 一般文字
#define QJGD_TEXT_SHOW  0x2000    // 区间轨道
#define XHJ_TEXT_SHOW   0x4000    // 信号机名称
#define SXLORR        0x80	//第八位表示上下行标志，1为左侧右行下行，0为左侧右行上行


//道岔右键菜单选项名称
#define MENU_DC  "定操"
#define MENU_FC    "反操"
#define MENU_DS    "单锁"
#define MENU_DJ    "单解"
#define MENU_FSJF    "封锁/解封"
#define MENU_QGJ    "区故解"
#define MENU_FLBL    "分路不良"
#define MENU_FLBL_CQ    "岔前 分路不良"
#define MENU_FLBL_DW    "定位 分路不良"
#define MENU_FLBL_FW    "反位 分路不良"
#define MENU_JGGZ    "尖轨故障"
#define MENU_XGGZ    "心轨故障"
#define MENU_JCW_DWWD    "接触网定位无电"
#define MENU_JCW_FWWD    "接触网反位无电"
#define MENU_QRKX    "确认空闲"
#define MENU_GDWD    "股道无电"
#define MENU_DJH_ADD    "添加调机号"
#define MENU_DJH_DEL    "删除调机号"
#define MENU_DJH_EDIT    "修改调机号"
#define MENU_JL_TG    "办理   通过进路"
#define MENU_JL_LC    "列车   进路办理"
#define MENU_JL_DC    "调车   进路办理"
#define MENU_JL_CANCLE    "取消进路"
#define MENU_XHREOPEN    "         信号重开"
#define MENU_JL_YD    "引导"
#define MENU_DDMD    "点灯/灭灯"
#define MENU_PDJS    "坡道解锁"
#define MENU_ZRS    "总人解"

#define MENU_STACHOSE    "车站选择"
#define MENU_STALOOK    "车站浏览"
#define MENU_MAINTOOL    "主工具条"
#define MENU_CTCBTN    "CTC按钮"
#define MENU_WORDSHOW    "文字显示"
#define MENU_JYJ    "绝缘节"
#define MENU_STAVIEW_REDUCTION    "还原 站场图"
#define MENU_STAVIEW_ENLARGE    "放大 站场图"
#define MENU_STAVIEW_NARROW    "缩小 站场图"
#define MENU_STAVIEW_MAX    "站场图全景"
#define MENU_CCC_REDUCTION    "还原 车次窗"
#define MENU_CCC_ENLARGE   "放大 车次窗"
#define MENU_CCC_NARROW    "缩小 车次窗"
#define MENU_CCC_POS    "车次窗位置"
#define MENU_CCC_SEEK    "查找车次窗"
#define MENU_JTGD    "接通光带"
#define MENU_JTGD_S    "继续接通光带30s"
#define MENU_LINEWIDT    "线条宽度"
#define MENU_XHJSIZE    "信号机大小"
#define MENU_BTN_LAPSIZE    "按钮和表示灯大小"

//QF 铅封类型
#define QF_GZTZ  0x01 //故障通知
#define QF_GDJS  0x02 //股道解锁
#define QF_GDFS  0x03 //股道封锁
#define QF_SDJS  0x04 //上电解锁
#define QF_SZRJ  0x05 //S总人解
#define QF_SYDZS 0x06 //S引导总锁
#define QF_XZRJ  0x07 //X总人解
#define QF_XYDZS 0x08 //X引导总锁
#define QF_XHDJLYD 0x09 //XHD进路引导
#define QF_SMD   0x0A //S灭灯
#define QF_XMD   0x0B //X灭灯
#define QF_ZDBS_ZFZ   0x10 //自动闭塞-总辅助
#define QF_ZDBS_JFZ   0x11 //自动闭塞-接辅助
#define QF_ZDBS_FFZ   0x12 //自动闭塞-发辅助
#define QF_ZDBS_GF    0x13 //自动闭塞-改方
#define QF_BZDBS_BS   0x14 //自动闭塞-闭塞
#define QF_BZDBS_SG   0x15 //自动闭塞-事故
#define QF_BZDBS_FY   0x16 //自动闭塞-复原
#define QF_PDJS    0x17 //坡道解锁
#define QF_SSGJS   0x18 //S事故解锁，即区段故障解锁
#define QF_XSGJS   0x19 //X事故解锁，即区段故障解锁
#define QF_FLBL    0x1A //分路不良
#define QF_SQGJ     0x1B //S区故解
#define QF_XQGJ     0x1C //X区故解

//低频发码
#define BSQDFM_H	0x01
#define BSQDFM_ZYJC   0x02
#define BSQDFM_HU	0x03
#define BSQDFM_ZPQH   0x04
#define BSQDFM_HB	0x05
#define BSQDFM_L4	0x06
#define BSQDFM_L6	0x07
#define BSQDFM_L5	0x08
#define BSQDFM_U2S	0x09
#define BSQDFM_USU	0x0A
#define BSQDFM_UU	0x0B
#define BSQDFM_U	0x0C
#define BSQDFM_LU2	0x0D
#define BSQDFM_U2	0x0E
#define BSQDFM_LU	0x0F
#define BSQDFM_L2	0x10
#define BSQDFM_L	0x11
#define BSQDFM_L3	0x12

//行车日志列号

/*定义工具条按钮是否按下标记 duxiaoxu 2017.4.15*/
#ifndef TOOL_H_H_
#define JL_BUILD		0x000001	//进路建立
#define ALL_CANCEL		0x000002    //总取消
#define SIGNAL_RESTART	0x000004	//信号重开
#define CALLON_BTN		0x000008    //引导按钮
#define CALLON_LOCK		0x000011    //引导总锁
#define ALLMAN_RELEASE	0x000012    //总人解
#define SECTION_RELEASE	0x000014    //区故解
#define SWITCH_NORMAL	0x000018    //道岔总定位
#define SWITCH_REVERSE	0x000021    //道岔总反位
#define SWITCH_LOCK		0x000022    //道岔单锁
#define SWITCH_RELEASE	0x000024    //道岔单解
#define LOCK_FUNCTION	0x000028    //封锁功能
#define UNLOCK_FUNCTION	0x000041    //解封功能
#define FUNC_BTN		0x000042    //功能按钮
#define RAMP_RELEASE	0x000048    //坡道解锁
#define CQBAD_SHUNT		0x000081    //岔前分路不良
#define DWBAD_SHUNT		0x000082    //定位分路不良
#define FWBAD_SHUNT		0x000084    //反位分路不良
#define BAD_SHUNT		0x000088    //分路不良
#define BAD_SHUNT_KX	0x000111    //分路不良空闲
#define LIGHT_ON    	0x000112    //点灯
#define LIGHT_OFF    	0x000114    //灭灯
#define FZ_LIST         0x000118    //辅助菜单
#define ORDER_CLEAR     0x000121    //命令清除
#define ORDER_GIVE      0x000122    //命令下达
#define STATE_CHOOSE    0x000124    //状态选择
#define MODLE_CHANGE    0x000128    //方式转换
#define POWEROFF_GD    	0x000141    //股道无电
#define POWEROFF_DCDW  	0x000142    //接触网定位无电
#define POWEROFF_DCFW 	0x000144    //接触网反位无电
#define DCFAULT_JG 	    0x000148    //道岔尖轨故障
#define DCFAULT_XG 	    0x000181    //道岔心轨故障
//new.2021.10.20.lwm
#define FUNC_GZJS	    0x020014    //故障解锁TK
#define FUNC_DCDF	    0x020018	//道岔单封TK
#define FUNC_DCJF	    0x020020	//道岔解封TK
#define FUNC_ANJIAF     0x020021	//按钮加封
#define FUNC_ANJIEF     0x020022	//按钮解封
#define QX_CQBAD_SHUNT  0x020024    //取消分路不良
#define POWERCUT_ON     0x020040    //停电
#define POWER_ON        0x020041    //供电
#define FUNC_TXSET      0x020048    //铁鞋防溜设置
#define FUNC_TXREMOVE   0x020050    //铁鞋防溜取消
#endif

#define ZFZ	    0x01    //总辅助
#define FCFZ	0x02	//发车辅助
#define JCFZ    0x03	//接车辅助
#define GF      0x04	//改方
//日志文件名称
extern QString G_LOG_FILE_NAME;

#endif // GLOBAL_H
