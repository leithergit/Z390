#ifndef QEVOLISPRINTER_H
#define QEVOLISPRINTER_H

#include <memory>
#include <QObject>
#include <thread>
#include <mutex>
#include <QApplication>
#include "LithographPrinter.h"

using namespace std;
typedef struct CardInfo
{
    std::string bankNumber;      //银行卡号
    std::string regionCode;      //区域代码
    std::string ATR;             //复位信息
    std::string algoCode = "03"; //算法标识
    std::string identifyNum;     //卡识别码
    std::string cardVersion;     //卡片版本
    std::string cardReleaseDate; //发卡日期
    std::string cardValidDate;   //卡有效期
    std::string cardNumber;      //卡号
    std::string cardID;          //身份证号
    std::string name;            //姓名
    std::string nameExtend;      //姓名拓展
    std::string sex;             //性别
    std::string nation;          //民族
    std::string birthday;        //出生日期
}CARDINFO;

using CardInfoPtr = shared_ptr<CardInfo>;

typedef struct NewCardInfo
{
    std::string bankNumber;      //银行卡号
    std::string regionCode;      //区域代码
    std::string ATR;             //复位信息
    std::string algoCode = "03"; //算法标识
    std::string identifyNum;     //卡识别码
    std::string cardVersion;     //卡片版本
    std::string cardReleaseDate; //发卡日期
    std::string cardValidDate;   //卡有效期
    std::string cardNumber;      //卡号
    std::string cardID;          //身份证号
    std::string name;            //姓名
    std::string nameExtend;      //姓名拓展
    std::string sex;             //性别
    std::string nation;          //民族
    std::string birthday;        //出生日期
}NEWCARDINFO;
using NewCardInfoPtr = shared_ptr<NewCardInfo>;

typedef struct PicInfo
{
    string  picpath;
    int     nAngle;
    float   fxPos;
    float   fyPos;
    float   fHeight;
    float   fWidth;
    int     nType;
    PicInfo()
    {
        memset(&nAngle,0,sizeof(PicInfo) - sizeof(picpath));
    }
}PICINFO;
using PicInfoPtr = shared_ptr<PicInfo>;

typedef struct TextInfo
{
    string  sText;
    int     nAngle; //旋转角度
    float   fxPos;
    float   fyPos;
    string  pFontName;
    int     nFontSize;
    int     nFontStyle;
    int     nColor;
    int     nType;
    TextInfo()
    {
        nAngle = 0;
        fxPos = 0;
        nFontSize = 0;
        nFontStyle = 0;
        nColor = 0;
        nType = 0;
    }
}TEXTINFO;

enum CardPostion
{
    Pos_NA = -1,
    Pos_Smart =  1,      // 读磁
    Pos_Contact ,        // 接触
    Pos_ContactLess,     // 非接触
    Pos_Print,           // 打印位
    Pos_EjectCard,       // 出卡口
    Pos_RejectCard       // 废卡箱
};
enum TaskType
{
    Task_Print_Open         ,
    Task_Print_Close        ,
    Task_Print_Reset        ,
    Task_Print_Eject        ,
    Task_Print_Retract      ,
    Task_Print_Dispense     ,
    Task_Print_GetBoxStatus ,
    Task_Print_Status       ,
    Task_Print_InitPrint    ,
    Task_Print_StartPrint
};

struct Task
{
    TaskType    nTaskType;
    //string      strTaskName;
    int         nResult;
    char*       pRCode;
    void        *pValue = nullptr;
    long        nTimeout;
    Task(TaskType nType,char *szRCode,long lTimeout = 0)
        :nTaskType(nType)
    {
        nResult = -1;
        pRCode = szRCode;
        nTimeout = lTimeout;
    }
    Task() = delete;

};

using TextInfoPtr = shared_ptr<TextInfo>;
using TaskPtr = shared_ptr<Task>;

class QEvolisPrinter
{
public:
    QEvolisPrinter();
    ~QEvolisPrinter();

    thread *pThread = nullptr;
    list<TaskPtr> listTask;
    mutex mtlistTask;
    mutex mtComplete;
    std::condition_variable cvComplete,cvTask;
    volatile bool bRunning = false;
    void run() ;
    int ProcessTask(TaskPtr pTask);
    int GetDeviceState(void  *pPrinter, char* majorStatusValue, char* minorStatusValue);
    int GetPrinterStatus(void  *pPrinter,int * RibbonNum,int *Device, int *Media);
    int PrintCard(PICINFO& inPicInfo, vector<TextInfoPtr>& inTextVector);
    int SetDarkTextRegion(int nLeft,int nTop,int nRight,int nBottom);
    PICINFO m_picInfo;
    vector<TextInfoPtr> m_textInfo;
    CardPostion     nCardPosition = Pos_NA;
    string  strOverlayer;
    string  strPreviewFile = "";
    string  strGRibbonType = "RM_YMCKO";
    string  strIFDarkLevelValue = "";
    void *m_pPrinter = nullptr;
    int  On_Print_Open(char *pPort, char *pPortParam, char *pszRcCode) ;
    int  On_Print_Close(char *pszRcCode) ;
    int  On_Print_Reset(long lTimeout, int nResetAction, char *pszRcCode) ;
    int  On_Print_Eject(long lTimeout, char *pszRcCode) ;
    int  On_Print_Retract(long lTimeout, int nBoxNo, char *pszRcCode) ;
    int  On_Print_Dispense(long lTimeout, int nBox, int nDispPos, char* pszRcCode) ;
    int  On_Print_GetBoxStatus(long lTimeout, Lithograph::LPLITHOGRAPHBOXINFO& lpBoxInfo, char* pszRcCode) ;
    int  On_Print_Status(long lTimeout, Lithograph::LPLITHOGRAPHSTATUS &lpStatus, char *pszRcCode) ;
    int  On_Print_InitPrint(long lTimeout, char *pszRcCode) ;
    int  On_Print_StartPrint(long lTimeout, char *pszRcCode) ;

    void Exit();

    int  Print_Open(char *pPort, char *pPortParam, char *pszRcCode);

    int  Print_Close(char *pszRcCode);

    int  Print_Reset(long lTimeout, int nResetAction, char *pszRcCode);

    int  Print_Eject(long lTimeout, char *pszRcCode);

    int  Print_Retract(long lTimeout, int nBoxNo, char *pszRcCode);

    int  Print_Dispense(long lTimeout, int nBox, int nDispPos, char* pszRcCode);

    int  Print_GetBoxStatus(long lTimeout, Lithograph::LPLITHOGRAPHBOXINFO& lpBoxInfo, char* pszRcCode);

    int  Print_Status(long lTimeout, Lithograph::LPLITHOGRAPHSTATUS &lpStatus, char *pszRcCode);

    int  Print_InitPrint(long lTimeout, char *pszRcCode);

    int  Print_StartPrint(long lTimeout, char *pszRcCode);
};

//class QEvolisCtrl:public QObject
//{
//    Q_OBJECT
//public:
//    QEvolisCtrl();

//    ~QEvolisCtrl();

//    void emitPrint_Open(int &nResult,char *pPort, char *pPortParam, char *pszRcCode)
//    {
//        emit Print_Open(nResult,pPort, pPortParam, pszRcCode);
//    }
//    void emitPrint_Close(int &nResult,char *pszRcCode)
//    {
//        emit Print_Close(nResult,pszRcCode);
//    }
//    void emitPrint_Reset(int &nResult,long lTimeout, int nResetAction, char *pszRcCode)
//    {
//        emitPrint_Reset(nResult,lTimeout, nResetAction, pszRcCode);
//    }
//    void emitPrint_Eject(int &nResult,long lTimeout, char *pszRcCode)
//    {
//        emit Print_Eject(nResult,lTimeout, pszRcCode);
//    }
//    void emitPrint_Retract(int &nResult,long lTimeout, int nBoxNo, char *pszRcCode)
//    {
//        emit Print_Retract(nResult,lTimeout, nBoxNo, pszRcCode);
//    }
//    void emitPrint_Dispense(int &nResult,long lTimeout, int nBox, int nDispPos, char* pszRcCode)
//    {
//        emit Print_Dispense(nResult,lTimeout, nBox, nDispPos, pszRcCode);
//    }
//    void emitPrint_GetBoxStatus(int &nResult,long lTimeout, Lithograph::LPLITHOGRAPHBOXINFO& lpBoxInfo, char* pszRcCode)
//    {
//        emit Print_GetBoxStatus(nResult,lTimeout, lpBoxInfo, pszRcCode);
//    }
//    void emitPrint_Status(int &nResult,long lTimeout, Lithograph::LPLITHOGRAPHSTATUS &lpStatus, char *pszRcCode)
//    {
//        emit Print_Status( nResult,lTimeout,lpStatus,pszRcCode);
//    }
//    void emitPrint_InitPrint(int &nResult,long lTimeout, char *pszRcCode)
//    {
//        emit Print_InitPrint(nResult,lTimeout, pszRcCode);
//    }
//    void emitPrint_StartPrint(int &nResult,long lTimeout, char *pszRcCode)
//    {
//        emit Print_StartPrint( nResult,lTimeout, pszRcCode);
//    }
//signals:
//    void  Print_Open(int &nResult,char *pPort, char *pPortParam, char *pszRcCode) ;
//    void  Print_Close(int &nResult,char *pszRcCode) ;
//    void  Print_Reset(int &nResult,long lTimeout, int nResetAction, char *pszRcCode) ;
//    void  Print_Eject(int &nResult,long lTimeout, char *pszRcCode) ;
//    void  Print_Retract(int &nResult,long lTimeout, int nBoxNo, char *pszRcCode) ;
//    void  Print_Dispense(int &nResult,long lTimeout, int nBox, int nDispPos, char* pszRcCode) ;
//    void  Print_GetBoxStatus(int &nResult,long lTimeout, Lithograph::LPLITHOGRAPHBOXINFO& lpBoxInfo, char* pszRcCode) ;
//    void  Print_Status(int &nResult,long lTimeout, Lithograph::LPLITHOGRAPHSTATUS &lpStatus, char *pszRcCode) ;
//    void  Print_InitPrint(int &nResult,long lTimeout, char *pszRcCode) ;
//    void  Print_StartPrint(int &nResult,long lTimeout, char *pszRcCode) ;
//};
#endif // QEVOLISPRINTER_H
