
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QtAndroid>
#include <jni.h>
#include <string>
#include <QPainter>
#include <QImage>
#include <QtDebug>
#include <string.h>
#include <QRectF>
#include <QRect>
#include <string>
#include <QtDebug>
#include <android/log.h>
#include <memory>
#include <QFileInfo>
#include <chrono>
#include <thread>
#include <cstdio>
#include <ctime>
#include <thread>
#include <QDir>
#include <QApplication>
#include <qglobal.h>
#include <QTextCodec>
#include "evolis.h"
#include "android.h"
#include "evo-printers.h"
#include "QEvolisPrinter.h"
#include "CVText_CN.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc_c.h>
using namespace cv;

struct BMP_INFOHDR
{
    unsigned long   biSize;
    long			biWidth;
    long			biHeight;
    unsigned short  biPlanes;
    unsigned short  biBitCount;
    unsigned long   biCompression;
    unsigned long   biSizeImage;
    long			biXPelsPerMeter;
    long			biYPelsPerMeter;
    unsigned long   biClrUsed;
    unsigned long   biClrImportant;
};
#pragma pack(push)
#pragma pack(1)
struct BMP_FILEHDR {                     // BMP file header
    char   bfType[2];                    // "BM"
    qint32  bfSize;                      // size of file
    qint16  bfReserved1;
    qint16  bfReserved2;
    qint32  bfOffBits;                   // pointer to the pixmap bits
};
#pragma pack(pop)


const int BMP_FILEHDR_SIZE = 14;               // size of BMP_FILEHDR data
const int BMP_WIN = 40;                        // Windows BMP v3 size
const int BMP_RGB = 0;                         // no compression
const int nCardWidth = 1024;
const int nCardHeight = 650;
using namespace std;
using namespace chrono;
#define LOBYTE(w)         ((unsigned char)(((ULONG)(w)) & 0xff))
#define Value_R(rgb)      (LOBYTE(rgb))
#define Value_G(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))
#define Value_B(rgb)      (LOBYTE((rgb)>>16))
#define MM2Pixel(W)        (W*DPI)/(25.4)

const int DPI = 300;

extern JavaVM *g_JVM ; // 保存虚拟机


#define RunlogF(...)    Runlog(__PRETTY_FUNCTION__,__LINE__,__VA_ARGS__);
#define Funclog()       FuncRunlog(__PRETTY_FUNCTION__,__LINE__);  auto threadID = std::this_thread::get_id();RunlogF("####Current ThreadID:%08x####\n",threadID);

#define     CheckPrinter(x)     if (!x) {\
                                RunlogF("Printer connection is invalid.\n");\
                                strcpy(pszRcCode, "0001");\
                                return 1;  }

#define __countof(array) (sizeof(array)/sizeof(array[0]))
//#pragma warning (disable:4996)
void Runlog(const char* pFunction,int nLine,const char* pFormat, ...)
{
    va_list args;
    va_start(args, pFormat);
    int nBuff;
    CHAR szBuffer[8192] = {0};
    nBuff = vsnprintf(szBuffer, __countof(szBuffer), pFormat, args);
    va_end(args);

    string strInfo = "Z390 ";
    strInfo += pFunction;
    strInfo += "@";
    strInfo += to_string(nLine);
    strInfo += ":";
    strInfo += szBuffer;
    __android_log_print(ANDROID_LOG_INFO,"Debug","%s",strInfo.c_str());
    //__android_log_vprint(ANDROID_LOG_INFO,"Debug",pFormat,args);
}
void FuncRunlog(const char* pFunction,int nLine)
{

    string strInfo = "Z390 ";
    strInfo += pFunction;
    strInfo += "@";
    strInfo += to_string(nLine);
    strInfo += ":";
    __android_log_print(ANDROID_LOG_INFO,"Debug","%s",strInfo.c_str());
}

int Convert_dib(char **pBuffer, const QImage& img)
{
    QImage image;
    switch (img.format())
    {
    case QImage::Format_Mono:
    case QImage::Format_Indexed8:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
        image = img;
        break;
    case QImage::Format_MonoLSB:
        image = img.convertToFormat(QImage::Format_Mono);
        break;
    case QImage::Format_Alpha8:
    case QImage::Format_Grayscale8:
        image = img.convertToFormat(QImage::Format_Indexed8);
        break;
    default:
        if (img.hasAlphaChannel())
            image = img.convertToFormat(QImage::Format_ARGB32);
        else
            image = img.convertToFormat(QImage::Format_RGB32);
        break;
    }

    qsizetype nSize = image.sizeInBytes();
    BMP_FILEHDR bf;
    BMP_INFOHDR bi;

    int nOffset = 0;
    char* pImageBuffer = new char[nSize + sizeof(bf) + sizeof(bf)];
    if (!pImageBuffer)
        return 0;

    int nbits;
    int bpl_bmp;
    // Calculate a minimum bytes-per-line instead of using whatever value this QImage is using internally.
    int bpl = ((image.width() * image.depth() + 31) >> 5) << 2;

    if (image.depth() == 8 && image.colorCount() <= 16)
    {
        bpl_bmp = (((bpl + 1) / 2 + 3) / 4) * 4;
        nbits = 4;
    }
    else if (image.depth() == 32)
    {
        bpl_bmp = ((image.width() * 24 + 31) / 32) * 4;
        nbits = 24;
    }
    else
    {
        bpl_bmp = bpl;
        nbits = image.depth();
    }
    // build file header
    memcpy(&bf.bfType, "BM", 2);

    // write file header
    bf.bfReserved1 = 0;
    bf.bfReserved2 = 0;
    bf.bfOffBits = BMP_FILEHDR_SIZE + BMP_WIN + image.colorCount() * 4;
    bf.bfSize = bf.bfOffBits + bpl_bmp * image.height();
    memcpy(&pImageBuffer[nOffset], &bf, sizeof(bf));
    nOffset += sizeof(bf);

    bi.biSize = BMP_WIN;                // build info header
    bi.biWidth = image.width();
    bi.biHeight = image.height();
    bi.biPlanes = 1;
    bi.biBitCount = nbits;
    bi.biCompression = BMP_RGB;
    bi.biSizeImage = bpl_bmp * image.height();
    bi.biXPelsPerMeter = image.dotsPerMeterX() ? image.dotsPerMeterX() : 2834; // 72 dpi default
    bi.biYPelsPerMeter = image.dotsPerMeterY() ? image.dotsPerMeterY() : 2834;
    bi.biClrUsed = image.colorCount();
    bi.biClrImportant = image.colorCount();

    memcpy(&pImageBuffer[nOffset], &bi, sizeof(bi));
    nOffset += sizeof(bi);


    if (image.depth() != 32)
    {                // write color table
        uchar* color_table = new uchar[4 * image.colorCount()];
        uchar* rgb = color_table;
        QVector<QRgb> c = image.colorTable();
        for (int i = 0; i < image.colorCount(); i++)
        {
            *rgb++ = qBlue(c[i]);
            *rgb++ = qGreen(c[i]);
            *rgb++ = qRed(c[i]);
            *rgb++ = 0;
        }
        memcpy(&pImageBuffer[nOffset], color_table, 4 * image.colorCount());
        nOffset += 4 * image.colorCount();

        delete []color_table;
    }

    int y;

    if (nbits == 1 || nbits == 8)
    {                // direct output
        for (y = image.height() - 1; y >= 0; y--)
        {
            memcpy(&pImageBuffer[nOffset], image.constScanLine(y), bpl);
            nOffset += bpl;
        }

        return nOffset;
    }

    uchar* buf = new uchar[bpl_bmp];
    uchar* b, * end;
    const uchar* p;

    memset(buf, 0, bpl_bmp);
    for (y = image.height() - 1; y >= 0; y--)
    {        // write the image bits
        if (nbits == 4)
        {                        // convert 8 -> 4 bits
            p = image.constScanLine(y);
            b = buf;
            end = b + image.width() / 2;
            while (b < end)
            {
                *b++ = (*p << 4) | (*(p + 1) & 0x0f);
                p += 2;
            }
            if (image.width() & 1)
                *b = *p << 4;
        }
        else
        {                                // 32 bits
            const QRgb* p = (const QRgb*)image.constScanLine(y);
            const QRgb* end = p + image.width();
            b = buf;
            while (p < end)
            {
                *b++ = qBlue(*p);
                *b++ = qGreen(*p);
                *b++ = qRed(*p);
                p++;
            }
        }
        memcpy(&pImageBuffer[nOffset], buf, bpl_bmp);
        nOffset += bpl_bmp;
    }
    delete[] buf;
    *pBuffer = pImageBuffer;
    return nOffset;
}

bool SetAndroidEnv()
{
    auto threadID = std::this_thread::get_id();
    RunlogF("####Current ThreadID:%08x####\n",threadID);

    if (!g_JVM)
    {
        RunlogF("JVM is invalid!.\n");
        return false;
    }
    JNIEnv *jniEnv;

    if (g_JVM->AttachCurrentThread(&jniEnv, NULL) != JNI_OK)
    {
        RunlogF("Failed  to AttachCurrentThread JVM Thread!\n");
        return false;
    }

    //RunlogF("to FindClass(android/app/ActivityThread\n");
    jclass activityThread = jniEnv->FindClass("android/app/ActivityThread");
    if (!activityThread)
    {
        RunlogF("Failed  to FindClass of jniEnv!.\n");
        return false;
    }
    //RunlogF("to GetStaticMethodID(()Landroid/app/ActivityThread)");
    jmethodID currentActivityThread = jniEnv->GetStaticMethodID(activityThread, "currentActivityThread", "()Landroid/app/ActivityThread;");
    if (!currentActivityThread)
    {
        RunlogF("Failed  to GetStaticMethodID of jniEnv!.\n");
        return false;
    }

    //RunlogF("to CallStaticObjectMethod");
    jobject jObj = jniEnv->CallStaticObjectMethod(activityThread, currentActivityThread);
    if (!jObj)
    {
        RunlogF("Failed  to CallStaticObjectMethod of jniEnv!.\n");
        return false;
    }

    //RunlogF("to GetMethodID(()Landroid/app/Application)");
    jmethodID jApplication = jniEnv->GetMethodID(activityThread, "getApplication", "()Landroid/app/Application;");
    if (!jApplication)
    {
        RunlogF("Failed  to GetMethodID of jniEnv!.\n");
        return false;
    }

    //RunlogF("to CallObjectMethod");
    jobject jContext = jniEnv->CallObjectMethod(jObj, jApplication);
    if (!jContext)
    {
        RunlogF("Failed  to CallObjectMethod of jniEnv!.\n");
        return false;
    }
    RunlogF("Try to evolis_set_android_env");
    evolis_set_android_env(jniEnv,jContext);

    return true;
}

//QEvolisCtrl::QEvolisCtrl()
//{
//    Funclog();

//}

//QEvolisCtrl::~QEvolisCtrl()
//{
//    Funclog();
//}

QApplication* pApp = nullptr;

QEvolisPrinter::QEvolisPrinter()
{
    Funclog();

//    const char *argv = "/data/app/com.example.crddriver_testtool-2/lib/arm/libEvolis_Z390_LithographPrinter_armeabi-v7a.so";
//    int argc = 1;

//    try
//    {
//        RunlogF("Try QApplication::addLibraryPath.");
////       qputenv("QT_ANDROID_FONTS","Roboto;Droid Sans;Droid Sans Fallback");
////       qputenv("QT_ANDROID_APP_ICON_SIZE","72");
////       qputenv("QT_USE_ANDROID_NATIVE_DIALOGS","1");
////       qputenv("QT_ANDROID_THEME","Theme_Holo_Light/");
////       qputenv("QT_ANDROID_THEME_DISPLAY_DPI","240");
////       qputenv("QT_BLOCK_EVENT_LOOPS_WHEN_SUSPENDED","1");
////       qputenv("QT_BUNDLED_LIBS_PATH","/data/app/com.example.crddriver_testtool-2/lib/arm/");
////       qputenv("MINISTRO_ANDROID_STYLE_PATH","/data/app/com.example.crddriver_testtool-2/qt-reserved-files/android-style/240/");
////       qputenv("QT_ANDROID_THEMES_ROOT_PATH","/data/app/com.example.crddriver_testtool-2/qt-reserved-files/android-style/");

//    catch (std::exception e)
//    {
//        RunlogF("An exception occured when new a QApplication Instance.\n");
//    }

    bRunning = true;
    pThread = new thread(&QEvolisPrinter::run,this);
    this_thread::sleep_for(std::chrono::milliseconds(250));
    ExtractIso();
    ExtractFont();
    strOverlayer = "/mnt/internal_sd/Z390/iso.bmp";
}

void QEvolisPrinter::ExtractIso()
{
    QFileInfo fi("/mnt/internal_sd/Z390/iso.bmp");
    if (fi.isFile())
    {
        RunlogF("File /mnt/internal_sd/Z390/iso.bmp has already exist!\n");
        return ;
    }
    RunlogF("/mnt/internal_sd/Z390/iso.bmp not exist,try to create...!\n");
    QFileInfo fidir("/mnt/internal_sd/Z390");
    if (!fidir.isDir())
    {
        RunlogF("Directory /mnt/internal_sd/Z390/ not exist,try to create...!\n");
        QDir dir;
        if(!dir.mkpath("/mnt/internal_sd/Z390/"))
        {
            RunlogF("Failed in creating directory /mnt/internal_sd/Z390 !\n");
            return ;
        }
    }
    RunlogF("Try to create file /mnt/internal_sd/Z390/iso.bmp!\n");
    if (!QFile::copy(":/iso.bmp","/mnt/internal_sd/Z390/iso.bmp"))
        RunlogF("Failed in creating file /mnt/internal_sd/Z390/iso.bmp!\n");

    strOverlayer = "/mnt/internal_sd/Z390/iso.bmp";
}

void QEvolisPrinter::ExtractFont()
{
    QFileInfo fi("/mnt/internal_sd/Z390/simsun.ttc");
    if (fi.isFile())
    {
        RunlogF("File /mnt/internal_sd/Z390/simsun.ttc has already exist!\n");
        return ;
    }
    RunlogF("/mnt/internal_sd/Z390/simsun.ttc not exist,try to create...!\n");
    QFileInfo fidir("/mnt/internal_sd/Z390");
    if (!fidir.isDir())
    {
        RunlogF("Directory /mnt/internal_sd/Z390/ not exist,try to create...!\n");
        QDir dir;
        if(!dir.mkpath("/mnt/internal_sd/Z390/"))
        {
            RunlogF("Failed in creating directory /mnt/internal_sd/Z390 !\n");
            return ;
        }
    }
    RunlogF("Try to create file /mnt/internal_sd/Z390/simsun.ttc!\n");
    if (!QFile::copy(":/simsun.ttc","/mnt/internal_sd/Z390/simsun.ttc"))
        RunlogF("Failed in creating file /mnt/internal_sd/Z390/simsun.ttc!\n");
}
QEvolisPrinter::~QEvolisPrinter()
{
    Funclog();
    g_JVM->DetachCurrentThread();
}

void QEvolisPrinter::Exit()
{
    Funclog();
    bRunning = false;
    cvTask.notify_one();
}

#define NotifyandWait()     {   RunlogF("Send notify for a new task.\n");\
                                cvTask.notify_one();\
                                unique_lock<mutex> lockComplete(mtComplete);\
                                RunlogF("Wait for task complete.\n");\
                                cvComplete.wait(lockComplete);\
                                RunlogF("Task complete,Result = %d,RcCode = %s.\n",pTask->nResult,pTask->pRCode);\
                                return pTask->nResult;\
                              }

int  QEvolisPrinter::Print_Open(char *pPort, char *pPortParam, char *pszRcCode)
{
    Funclog();
    TaskPtr pTask = make_shared<Task>(Task_Print_Open,pszRcCode);
    RunlogF("Put a task.\n");
    unique_lock<mutex> lock(mtlistTask);
    listTask.push_back(pTask);
    lock.unlock();
    RunlogF("Send notify for a new task.\n");
    cvTask.notify_one();
    unique_lock<mutex> lockComplete(mtComplete);
    RunlogF("Wait for task complete.\n");
    cvComplete.wait(lockComplete);
    RunlogF("Task complete,Result = %d,RcCode = %s.\n",pTask->nResult,pTask->pRCode);
    return pTask->nResult;
}

int  QEvolisPrinter::Print_Close(char *pszRcCode)
{
    Funclog();

    CheckPrinter(m_pPrinter);

    TaskPtr pTask = make_shared<Task>(Task_Print_Close,pszRcCode);
    unique_lock<mutex> lock(mtlistTask);
    listTask.push_back(pTask);
    lock.unlock();
    NotifyandWait();
}

int  QEvolisPrinter::Print_Reset(long lTimeout, int nResetAction, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_Reset,pszRcCode);
    pTask->pValue = &nResetAction;
    unique_lock<mutex> lock(mtlistTask);
    listTask.push_back(pTask);
    lock.unlock();
    NotifyandWait();
}

// 退卡到门口
int  QEvolisPrinter::Print_Eject(long lTimeout, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_Eject,pszRcCode,lTimeout);
    unique_lock<mutex> lock(mtlistTask);
    listTask.push_back(pTask);
    lock.unlock();
    NotifyandWait();
}

int  QEvolisPrinter::Print_Retract(long lTimeout, int nBoxNo, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_Retract,pszRcCode,lTimeout);
    pTask->pValue = &nBoxNo;
    unique_lock<mutex> lock(mtlistTask);
    listTask.push_back(pTask);
    lock.unlock();
    NotifyandWait();
}

int  QEvolisPrinter::Print_Dispense(long lTimeout, int nBox, int nDispPos, char* pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_Dispense,pszRcCode,lTimeout);
    int nArray[2] = {nBox,nDispPos};
    pTask->pValue = nArray;
    unique_lock<mutex> lock(mtlistTask);
    listTask.push_back(pTask);
    lock.unlock();
    NotifyandWait();
}

int  QEvolisPrinter::Print_GetBoxStatus(long lTimeout, Lithograph::LPLITHOGRAPHBOXINFO& lpBoxInfo, char* pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_GetBoxStatus,pszRcCode,lTimeout);
    pTask->pValue = &lpBoxInfo;
    unique_lock<mutex> lock(mtlistTask);
    listTask.push_back(pTask);
    lock.unlock();
    NotifyandWait();
}

int  QEvolisPrinter::Print_Status(long lTimeout, Lithograph::LPLITHOGRAPHSTATUS &lpStatus, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_Status,pszRcCode,lTimeout);
    pTask->pValue = &lpStatus;
    unique_lock<mutex> lock(mtlistTask);
    listTask.push_back(pTask);
    lock.unlock();
    NotifyandWait();
}

int  QEvolisPrinter::Print_InitPrint(long lTimeout, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_InitPrint,pszRcCode,lTimeout);
    unique_lock<mutex> lock(mtlistTask);
    listTask.push_back(pTask);
    lock.unlock();
    NotifyandWait();
}

int  QEvolisPrinter::Print_StartPrint(long lTimeout, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_StartPrint,pszRcCode,lTimeout);
    unique_lock<mutex> lock(mtlistTask);
    listTask.push_back(pTask);
    lock.unlock();
    NotifyandWait();
}

void QEvolisPrinter::run()
{
    if (bRunning)
    {
        Funclog();
        if (!SetAndroidEnv())
             return ;
    }
    else
        return;

    while(bRunning)
    {
        unique_lock<mutex> locklist(mtlistTask);
        RunlogF("Waiting for a new Task.\n");
        cvTask.wait(locklist);
        if (listTask.size())
        {
            RunlogF("Get a Task,listTask size:%d.\n",listTask.size());
            TaskPtr pTask = listTask.front();
            listTask.pop_front();
            RunlogF("ProcessTask a Task.\n");
            pTask->nResult = ProcessTask(pTask);
            RunlogF("Task Finished.\n");
        }
        //unique_lock<mutex> CompleteLock(mtComplete);
        RunlogF("Try to Send notify to Requestor.\n");
        cvComplete.notify_one();
    }
    RunlogF("JVM deatch current thread.\n");
    g_JVM->DetachCurrentThread();

}

int QEvolisPrinter::ProcessTask(TaskPtr pTask)
{
    Funclog();
    RunlogF("Task Type :%d.\n",pTask->nTaskType);
    switch (pTask->nTaskType)
    {
    case Task_Print_Open         :
        return On_Print_Open("evolis_zenius","",pTask->pRCode);
    case Task_Print_Close        :
        return On_Print_Close(pTask->pRCode);
    case Task_Print_Reset        :
        return On_Print_Reset(pTask->nTimeout,*((int*)pTask->pValue), pTask->pRCode);
    case Task_Print_Eject        :
        return On_Print_Eject(pTask->nTimeout, pTask->pRCode);
    case Task_Print_Retract      :
        return On_Print_Retract(pTask->nTimeout,*((int*)pTask->pValue), pTask->pRCode);
    case Task_Print_Dispense     :
    {
        int *pArray = (int*)pTask->pValue;
        return On_Print_Dispense(pTask->nTimeout,pArray[0],pArray[1], pTask->pRCode);
    }
    case Task_Print_GetBoxStatus :
    {
        Lithograph::LPLITHOGRAPHBOXINFO pBoxinfo = *(Lithograph::LPLITHOGRAPHBOXINFO *)pTask->pValue;
        return On_Print_GetBoxStatus(pTask->nTimeout,pBoxinfo, pTask->pRCode);
    }
    case Task_Print_Status       :
    {
        Lithograph::LPLITHOGRAPHSTATUS pStatus = *(Lithograph::LPLITHOGRAPHSTATUS*)pTask->pValue;
        return On_Print_Status(pTask->nTimeout,pStatus, pTask->pRCode);
    }
    case Task_Print_InitPrint    :
        return On_Print_InitPrint(pTask->nTimeout, pTask->pRCode);
    case Task_Print_StartPrint   :
        return On_Print_StartPrint(pTask->nTimeout, pTask->pRCode);
    }
}

int  QEvolisPrinter::On_Print_Open(char *pPort, char *pPortParam, char *pszRcCode)
{
    Funclog();
    evolis_device_t* devices;
    RunlogF("Try to evolis_get_devices.\n");
    if (evolis_get_devices(&devices, 0, 0) <= 0)
    {
        strcpy(pszRcCode, "0001");
        return  1;
    }
    RunlogF("evolis_get_devices Succeed.\n");

    RunlogF("Try to call evolis_open.\n");
    m_pPrinter = evolis_open(L"Evolis Zenius", false);
    if (!m_pPrinter)
    {
        strcpy(pszRcCode, "0001");
        return 1;
    }
    const char *szCmd="Pcim;F";
    char szReply[128] = {0};

    RunlogF("Try to evolis_command(%s).\n",szCmd);
    if (evolis_command(m_pPrinter,szCmd,strlen(szCmd),szReply,128) < 0)
    {
        strcpy(pszRcCode, "0001");
        RunlogF("Failed in evolis_command(%s):%s.\n",szCmd);
        return 1;
    }
    RunlogF("evolis_command(%s) Succeed,Reply:%s.\n",szCmd,szReply);

    RunlogF("evolis_open Succeed.\n");
    strcpy(pszRcCode, "0000");
    return 0;
}
int  QEvolisPrinter::On_Print_Close(char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);

    RunlogF("Try to evolis_close.\n");
    evolis_close(m_pPrinter);
    RunlogF("evolis_close Succeed.\n");
    m_pPrinter = nullptr;

    strcpy(pszRcCode, "0000");
    return 0;
}

int  QEvolisPrinter::On_Print_Reset(long lTimeout, int nResetAction, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    char timeouted;
    if (m_pPrinter)
    {
        RunlogF("Try to evolis_reset.\n");
        evolis_reset(m_pPrinter, lTimeout, &timeouted);
        RunlogF("evolis_reset Succeed.\n");
        switch (nResetAction)
        {
        default:
        case 1:
        {
            strcpy(pszRcCode, "0003");
            return 1;
            break;
        }
        case 2:
        {
            RunlogF("Try to evolis_set_card_pos(EVOLIS_CP_EJECT).\n");
            if (!evolis_set_card_pos(m_pPrinter, evolis_pos_t::EVOLIS_CP_EJECT))
            {
                strcpy(pszRcCode, "0000");
                return  0;
            }
            else
            {
                strcpy(pszRcCode, "0002");
                 return  1;
            }
        }
        case 3:
        {
//            RunlogF("Try to evolis_set_card_pos(EVOLIS_CP_REJECT).\n");
//            if (!evolis_set_card_pos(m_pPrinter, evolis_pos_t::EVOLIS_CP_REJECT))
//            {
//                strcpy(pszRcCode, "0000");
//                 return 0;
//            }
//            else
//            {
//                strcpy(pszRcCode, "0002");
//                return 1;
//            }
            // 吞卡到废卡箱
            char RespValue[1024];
            memset(RespValue, 0x00, 1024);
            const char* szCommand[] = { "Pneab;A;1","Pcem;N","Se" };

            int Res = 0;
            for (auto var : szCommand)
            {
                Res = evolis_command(m_pPrinter, var,strlen(var), RespValue,1024);
                if (Res < 0){
                    RunlogF("evolis_command(%s)failed.\n",var);
                    strcpy(pszRcCode, "0002");
                    return 1;
                }
            }
            nCardPosition = Pos_EjectCard;
            break;
        }
        }
        nCardPosition = (CardPostion)nResetAction;
        strcpy(pszRcCode, "0000");
        return 0;
    }
    else
    {
        strcpy(pszRcCode, "0001");
        return 1;
    }
     return 0;
}
int  QEvolisPrinter::On_Print_Eject(long lTimeout, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);

    char szReply[1024] = { 0 };
    const char* szCommand[] = {"Pneab;A;1","Pcem;M"};
    for (auto var :szCommand)
    {
        RunlogF("Try to evolis_command(%s).\n",var);
        int error = evolis_command(m_pPrinter, var,strlen(var), szReply,1024);
        if (error < 0)
        {
            strcpy(pszRcCode, "0001");
            RunlogF("Failed in evolis_command(%s).\n",var);
            return 1;
        }
         RunlogF("evolis_command(%s) Succeed,Reply:%s.\n",var,szReply);
    }

    RunlogF("Try to evolis_set_card_pos(EVOLIS_CP_EJECT).\n");
    if (evolis_set_card_pos(m_pPrinter,EVOLIS_CP_EJECT))
    {
        strcpy(pszRcCode, "0001");
        RunlogF("Failed in evolis_set_card_pos(EVOLIS_CP_EJECT).\n");
        return 1;
    }
    RunlogF("evolis_set_card_pos(EVOLIS_CP_EJECT) Succeed.\n");
    nCardPosition = Pos_EjectCard;
    strcpy(pszRcCode, "0000");
    return 0;
}

int  QEvolisPrinter::On_Print_Retract(long lTimeout, int nBoxNo, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    char RespValue[1024];
    memset(RespValue, 0x00, 1024);
    const char* szCommand[] = { "Pneab;A;1","Pcem;N","Se" };

    int Res = 0;
    for (auto var : szCommand)
    {
        RunlogF("Try to evolis_command(%s).\n",var);
        Res = evolis_command(m_pPrinter, var,strlen(var), RespValue,1024);
        if (Res < 0){
            RunlogF("evolis_command(%s)failed.\n",var);
            strcpy(pszRcCode, "0002");
            return 1;
        }
         RunlogF("evolis_command(%s) Succeed,Reply:%s.\n",var,RespValue);
    }
    nCardPosition = Pos_EjectCard;
    strcpy(pszRcCode, "0000");

    //		if (0 == evolis_set_card_pos(m_pPrinter, EVOLIS_CP_SCAN))
    //		{
    //            nCardPosition = 0;
    //			strcpy(pszRcCode, "0000");
    //			return 0;
    //		}
    //		else
    //		{
    //            RunlogF("Failed in evolis_set_card_pos(EVOLIS_CP_SCAN).\n");
    //            strcpy(pszRcCode, "0002");
    //			return 1;
    //		}
    return  0;
}
int  QEvolisPrinter::On_Print_Dispense(long lTimeout, int nBox, int nDispPos, char* pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);

    RunlogF("Try to evolis_set_input_tray(EVOLIS_IT_FEEDER).\n");
    int nRes = evolis_set_input_tray(m_pPrinter,EVOLIS_IT_FEEDER);
    if (nRes)
    {
        strcpy(pszRcCode, "0001");
        RunlogF("Failed in evolis_set_input_tray(EVOLIS_IT_FEEDER).\n");
        return 1;
    }
    RunlogF("evolis_set_input_tray(EVOLIS_IT_FEEDER) Succeed.\n");

    RunlogF("Try to evolis_set_card_pos(EVOLIS_CP_INSERT).\n");
    nRes = evolis_set_card_pos(m_pPrinter,EVOLIS_CP_INSERT);
    if (nRes)
    {
        strcpy(pszRcCode, "0001");
        RunlogF("Failed in evolis_set_card_pos(EVOLIS_CP_INSERT).\n");
        return 1;
    }
    RunlogF("evolis_set_card_pos(EVOLIS_CP_INSERT) Succeed.\n");

    this_thread::sleep_for(std::chrono::milliseconds(200));
    RunlogF("Try to evolis_set_card_pos(EVOLIS_CP_CONTACT).\n");
    nRes = evolis_set_card_pos(m_pPrinter,EVOLIS_CP_CONTACT);
    if (nRes)
    {
        strcpy(pszRcCode, "0001");
        RunlogF("Failed in evolis_set_card_pos(EVOLIS_CP_CONTACT).\n");
        return 1;
    }
    RunlogF("evolis_set_card_pos(EVOLIS_CP_CONTACT) Succeed.\n");
    nCardPosition = Pos_Contact;
    strcpy(pszRcCode, "0000");
    return 0;
}

int  QEvolisPrinter::On_Print_GetBoxStatus(long lTimeout, Lithograph::LPLITHOGRAPHBOXINFO& lpBoxInfo, char* pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    const char *szCmdArray[1] = {"Rlr;p"};
    char szReplyArray[2][128] = {{0},{0}};
    for (int i = 0;i < 2;i++)
    {
        RunlogF("Try to evolis_command(%s).\n",szCmdArray[i]);
        if (evolis_command(m_pPrinter, szCmdArray[i],5, szReplyArray[i],128))
        {
            RunlogF("evolis_command(%s) Failed.\n",szCmdArray[i]);
        }
        else
        {
            RunlogF("evolis_command(%s) Succeed,Reply:%s.\n",szReplyArray[i]);
        }
    }
    const char *szCmd="Rse;f";
    char szReply[128] = {0};
    // chechk the card box status
    RunlogF("Try to evolis_command(%s).\n",szCmd);
    //卡箱无卡时：evolis_command(Rse;f) 返回:0,00 Volts
    //卡箱有卡时：evolis_command(Rse;f) 返回:5,00 Volts，这个可以对上了
    if (evolis_command(m_pPrinter,szCmd,strlen(szCmd),szReply,128) < 0)
    {
        RunlogF("Faild in evolis_command(Rse;f),Reply:%s",szReply);
        strcpy(pszRcCode, "0001");
        return 1;
    }
    RunlogF("evolis_command(%s) succeed,Reply:%s",szCmd,szReply);

    lpBoxInfo->nCount = 2;
    lpBoxInfo->lpplist = new Lithograph::LITHOGRAPHBOXUNIT[2];
    lpBoxInfo->lpplist[0].byType = 1;           //卡箱类型 0:未知，1：发卡箱，2：回收箱
    lpBoxInfo->lpplist[0].byBoxNumber = 1;      //卡箱号
    lpBoxInfo->lpplist[0].bHardwareSensor = false;//是否支持故障状态传感器,TRUE:支持，FALSE：不支持
    if (szReply[0] == '5')
        lpBoxInfo->lpplist[0].byStatus = 0;     //卡箱状态,0：正常; 1：卡少; 2:无卡; 3：不可操作; 4：不存在; 5：高(快满)；6：满； 7：未知
    else
        lpBoxInfo->lpplist[0].byStatus = 2;

    lpBoxInfo->lpplist[1].byType = 2;
    lpBoxInfo->lpplist[1].byBoxNumber = 2;
    lpBoxInfo->lpplist[1].byStatus = 0;
    lpBoxInfo->lpplist[1].bHardwareSensor = false;

    char majorStatusValue[1024] = {0};
    char minorStatusValue[1024] = {0};
    if (GetDeviceState(m_pPrinter, majorStatusValue, minorStatusValue))
    {
        RunlogF("Failed in GetDeviceState.\n");
        strcpy(pszRcCode, "0002");
        return 1;
    }

    RunlogF("majorStatusValue = %s.\n",majorStatusValue);
    RunlogF("minorStatusValue = %s.\n",minorStatusValue);

    if(strcmp(minorStatusValue,"STDEVOLIS_DEF_FEEDER_EMPTY") == 0)
    {
        lpBoxInfo->lpplist[0].byStatus = 2;
    }
    else
    {
        lpBoxInfo->lpplist[0].byStatus = 0;
    }

    strcpy(pszRcCode, "0000");

    return 0;
}

//WORD fwDevice;		//打印机状态,    0-在线；   1-忙；     2-不在线；3-故障
//WORD fwMedia;			//介质状态，     0-无卡；   1-卡在门口；2-卡在内部；3-卡在上电位，4-卡在闸门外；5-堵卡；6-卡片未知（根据硬件特性返回,必须支持有无卡检测）
//WORD fwToner;			//平印色带状态,  0-FLLL;  1-LOW;2-OUT;3-NOTSUPP;4-UNKNOW
int  QEvolisPrinter::On_Print_Status(long lTimeout, Lithograph::LPLITHOGRAPHSTATUS &lpStatus, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);

    int RibbonNum = 0;
    int fwDevice = 0;
    int fwMedia = 0;

    if (0 == GetPrinterStatus(m_pPrinter,&RibbonNum,&fwDevice,&fwMedia))
    {
        if (RibbonNum == -98)
            lpStatus->fwToner = 4;
        else
        {
            if (RibbonNum <=20 )
                lpStatus->fwToner = 0;
            else
                lpStatus->fwToner = 1;
        }

        lpStatus->fwDevice = fwDevice;
        lpStatus->fwMedia = fwMedia;
        strcpy(pszRcCode, "0000");
        return 0;
    }
    else
    {
        lpStatus->fwDevice = 2;
        strcpy(pszRcCode, "0002");
        return 1;
    }
}

int  QEvolisPrinter::On_Print_InitPrint(long lTimeout, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);

    m_textInfo.clear();
    memset(&m_picInfo, 0x00, sizeof(PICINFO));

    //发送Pneab;E
    const char* pCommand = "Pneab;E";
    char szReply[1024] = { 0 };
    int nRes = 1;

    RunlogF("Try to evolis_command(Pneab;E).\n");
    nRes = evolis_command(m_pPrinter,pCommand, strlen(pCommand), szReply, 1024);
    if (nRes < 0)
    {
        RunlogF("发送命令失败.\n");
        strcpy(pszRcCode, "0002");
        return 1;
    }
    RunlogF("evolis_command(Pneab;E) Succeed,Reply:%s.\n",szReply);

    this_thread::sleep_for(chrono::milliseconds(10));//1毫秒

    RunlogF("Try to evolis_print_init.\n");
    if (evolis_print_init(m_pPrinter))
    {
        RunlogF("evolis_print_init Failed.\n");
        strcpy(pszRcCode, "0002");
        return 1;
    }
    RunlogF("evolis_print_init Succeed.\n");
    strcpy(pszRcCode, "0000");
    return 0;
}

int  QEvolisPrinter::On_Print_StartPrint(long lTimeout, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);

    if (!m_textInfo.size())
        RunlogF("There is no Text info.\n");
    for (size_t i = 0; i < m_textInfo.size(); i++)
    {
        char temp[1024];
        sprintf(temp, "%s,%f-%f", m_textInfo[i]->sText.c_str(), m_textInfo[i]->fxPos, m_textInfo[i]->fyPos);
        //logF(temp);
    }

    if (m_textInfo.size() <= 0 && m_picInfo.picpath.size() <= 0)
    {
        RunlogF("There is no Image or Text info.\n");
        strcpy(pszRcCode, "0001");
        return 1;
    }
    char szReply[1024] = {0};
    const char* szCommand =  "Pneab;E";
    RunlogF("Try to evolis_command(%s).\n",szCommand);
    if (evolis_command(m_pPrinter, szCommand, strlen(szCommand), szReply, sizeof(szReply)) < 0)
    {
        RunlogF("evolis_command(%s) Failed.\n",szCommand);
        strcpy(pszRcCode, "0001");
        return 1;
    }
    RunlogF("evolis_command(%s) Succeed,Reply:%s.\n",szCommand,szReply);
    int ec = PrintCard(m_picInfo, m_textInfo);
    if (ec)
    {
        strcpy(pszRcCode, "0001");
        return 1;
    }
    else
    {
        strcpy(pszRcCode, "0000");
        return  0;
    }
}

int QEvolisPrinter::GetDeviceState(void  *pPrinter, char* majorStatusValue, char* minorStatusValue)
{
    Funclog();
    if (!pPrinter)
        return 1;

    evolis_major_state_t major;
    evolis_minor_state_t minor;
    if (evolis_get_state(pPrinter, &major, &minor))
    {
        RunlogF("Failed in evolis_get_state.\n");
        return 1;
    }

    strcpy(majorStatusValue, evolis_get_major_string(major));
    strcpy(minorStatusValue,evolis_get_minor_string(minor));

    return 0;
}

//   WORD fwDevice;		//打印机状态,   需要有0和1和2，0在线，1有卡在内部工作中，2离线,2021.06.09 13.21 胡杰 定义
//   WORD fwMedia;		//介质状态，     0-无卡；2-卡在内部；2021.06.09 13.21 胡杰 定义
//   WORD fwToner;		//平印色带状态,  0-FLLL;1-LOW，4-未知 2021.06.09 13.38 胡杰 定义
/*
Rlr;p的对应结果
机器内没卡-->OK
卡在打印位-->CARD
卡在芯片位-->SMART CARD
卡在非接位-->CONTACTLESS CARD

Rse;M
0.2x Volts是没卡
4.9x Volts是有卡
*/
int QEvolisPrinter::GetPrinterStatus(void  *pPrinter,int * RibbonNum,int *Device, int *Media)
{
    Funclog();
    if (!pPrinter)
        return 1;

    *Media = 0;        // No card

    DWORD  ec;
    evolis_ribbon_t ribbon;
    /// Get ribbon infos.
    RunlogF("Try to evolis_get_ribbon.\n");
    ec = evolis_get_ribbon(pPrinter, &ribbon);
    if (ec != 0)
    {
        RunlogF("Failed in evolis_get_ribbon.\n");
        return 1;
    }
    RunlogF("ribbon.capacity = %d\tribbon.remaining = %d.\n",ribbon.capacity,ribbon.remaining);
    *RibbonNum= ribbon.capacity - ribbon.remaining;

    char majorStatusValue[1024] = {0};
    char minorStatusValue[1024] = {0};
    RunlogF("Try to GetDeviceState.\n");
    ec = GetDeviceState(pPrinter, majorStatusValue, minorStatusValue);
    if (ec != 0)
    {
        RunlogF("Failed in GetDeviceState.\n");
        return 1;
    }

    // 无卡        majorStatusValue = READY         minorStatusValue = DEVICE_READY
    // 卡在机器内部  majorStatusValue = WARNING       minorStatusValue = DEVICE_BUSY
    // 卡在出卡口    majorStatusValue = WARNING       minorStatusValue = STDEVOLIS_DEF_CARD_ON_EJECT
    //             majorStatusValue = WARNING       minorStatusValue = EVOLIS_MI_STDEVOLIS_DEF_COVER_OPEN

    //
    RunlogF("majorStatusValue = %s\tminorStatusValue = %s.\n",majorStatusValue,minorStatusValue);
    if (strcmp(minorStatusValue, "DEF_UNSUPPORTED_RIBBON") == 0)
        *RibbonNum = -98;//是否离线

    if(strcmp(minorStatusValue,"PRINTER_OFFLINE") == 0)
        *Device = 2;//是否离线

    //Media 0-无卡；1-卡在门口；2-卡在内部；3-卡在上电位，4-卡在闸门外；5-堵卡；6-卡片未知（根据硬件特性返回,必须支持有无卡检测）
    evolis_status_t es;
    RunlogF("Try to evolis_status.\n");
    if (evolis_status(pPrinter, &es) != 0)
    {
        RunlogF("Error evolis_status.\n");
        *Device = 2;    // 离线
    }
    else
    {
        *Device = 0;    // 在线
        RunlogF("evolis_status return:config = %d\tinformation = %d\twarning = %d\terror = %d.\n", es.config,es.information, es.warning,es.error);
        for (int i = 0;i < EVOLIS_STATUS_EX_COUNT;i ++)
            RunlogF("evolis_status return exts[%d] = %d.\n",i, es.exts[i]);
    }

    if (!strcmp(majorStatusValue,"READY") && !strcmp(minorStatusValue,"DEVICE_READY"))
        *Media = 0;         // No card
    else if (strcmp(majorStatusValue,"WARNING") == 0)
    {
        if (strcmp(minorStatusValue,"DEVICE_BUSY") == 0)
        {
            *Device = 1;    // 有卡在内部工作中
            *Media = 2;     // card is internal
        }
        else if (strcmp(minorStatusValue,"STDEVOLIS_DEF_CARD_ON_EJECT") == 0)
        {
            *Media = 4;     // card is at exit
        }
        else
            *Media = 6;
    }
    else
         *Media = 6;

    return 0;
}

int QEvolisPrinter::SetDarkTextRegion(int nLeft, int nTop, int nRight, int nBottom)
{
    if (!m_pPrinter)
        return 1;

    if (nLeft < 0 || nTop <0 || nRight < 0 || nBottom<0 )
        return 1;

    if (evolis_print_set_option(m_pPrinter, "FBlackManagement", " TEXTINBLACK"))
    {
         RunlogF("Failed in evolis_print_set_option(FBlackManagement,TEXTINBLACK).\n");
         return 1;
    }
    RunlogF("try to evolis_print_set_option(IFTextRegion,%s).\n","0x0x1000x600");
    evolis_print_set_option(m_pPrinter, "IFTextRegion", "0x0x1000x600");
//    char szRegion[128] = {0};
//    sprintf(szRegion,"%dx%dx%dx%d",nLeft,nTop,nRight,nBottom);
//    RunlogF("try to evolis_print_set_option(IFTextRegion,%s).\n",szRegion);
//    if (evolis_print_set_option(m_pPrinter, "IFTextRegion", szRegion))
//    {
//         RunlogF("Failed in evolis_print_set_option(IFTextRegion,%s).\n",szRegion);
//         return 1;
//    }

    RunlogF("Try to evolis_print_set_option(IFBlackLevelValue, 40).\n");
    if (evolis_print_set_option(m_pPrinter, "IFBlackLevelValue", "40"))
    {
         RunlogF("Failed in evolis_print_set_option(IFBlackLevelValue,40).\n");
         return 1;
    }
//    if (evolis_print_set_option(m_pPrinter, "IFDarkLevelValue", "10"))
//    {
//        RunlogF("Failed in evolis_print_set_option(IFDarkLevelValue,10).\n");
//        return 1;
//    }
}

//int QEvolisPrinter::PrintCard(PICINFO& inPicInfo, vector<TextInfoPtr>& inTextVector)
//{
//    Funclog();
//    if (!m_pPrinter)
//        return 1;

//     QFileInfo fipic(inPicInfo.picpath.c_str());
//     if (!fipic.isFile() && !inTextVector.size())
//     {
//         RunlogF("There is no any content to print!\n")
//         return 1;
//     }

//    RunlogF("Try to evolis_print_init.\n");
//    if (evolis_print_init((evolis_t*)m_pPrinter))
//    {
//        RunlogF("evolis_print_init failed.\n");
//        return 1;
//    }
//    RunlogF("evolis_print_init Succeed.\n");
//    QImage Convas(1024, 650, QImage::Format_RGB888);
//    Convas.fill(Qt::white);
//    QPainter painter(&Convas);
//    painter.setCompositionMode(QPainter::CompositionMode_Source);
//    QImage FileImage;

//    RunlogF("Try to load file %s.\n",inPicInfo.picpath.c_str());
//    if (FileImage.load(QString::fromStdString( inPicInfo.picpath)))
//    {
//        RunlogF("Source  Graph:%s\txPos = %.1f\tyPos = %.1f\tAngle = %d.\n",inPicInfo.picpath.c_str(),inPicInfo.fxPos,inPicInfo.fyPos,inPicInfo.nAngle);
//        RunlogF("Convert Graph:%s\txPos = %.1f\tyPos = %.1f\tAngle = %d.\n",inPicInfo.picpath.c_str(),MM2Pixel(inPicInfo.fxPos),MM2Pixel(inPicInfo.fyPos),inPicInfo.nAngle);

//        painter.save();
//        painter.translate(nCardWidth/2,nCardHeight/2);
//        painter.rotate(inPicInfo.nAngle);
//        painter.translate(-nCardWidth/2,-nCardHeight/2);
//        painter.drawImage(QRect(MM2Pixel(inPicInfo.fxPos),MM2Pixel(inPicInfo.fyPos),MM2Pixel(inPicInfo.fWidth),MM2Pixel(inPicInfo.fHeight)),FileImage);
//        painter.restore();
//    }
//    else
//        RunlogF("Failed in load file :%s.\n",inPicInfo.picpath.c_str());

//    for (auto var : inTextVector)
//    {
//        QString imageText = QString::fromStdString(var->sText);
//        QFont font(var->pFontName.c_str());
//        //设置显示字体的大小
//        font.setPixelSize(MM2Pixel(var->nFontSize));
//        //1-常规；2-粗体；4-斜体；8-黑体
//        if (var->nFontStyle &0x02)
//            font.setBold(true);
//        if (var->nFontStyle & 0x04)
//            font.setItalic(true);

//        RunlogF("Font Name:%s.\n",var->pFontName.c_str());
//        QFontMetrics fm(font);
//        auto TextRect = fm.boundingRect(imageText);
//        painter.setFont(font);
//        QPen pen = painter.pen();

//        pen.setColor(QColor(Value_R(var->nColor),Value_G(var->nColor),Value_B(var->nColor)));
//        painter.setPen(pen);
//        RunlogF("Source Text:%s\n",var->sText.c_str());
//        //RunlogF("Source:xPos = %.1f\tyPos = %.1f\tAngle = %d\nConvert Text:%s\txPos = %.1f\tyPos = %.1f\tAngle = %d.\n.\n",
//        //        (var->fxPos),(var->fyPos),var->nAngle,
//        //        MM2Pixel(var->fxPos),MM2Pixel(var->fyPos),var->nAngle);

//        painter.save();
//        painter.translate(nCardWidth/2,nCardHeight/2);
//        painter.rotate(var->nAngle);
//        painter.translate(-nCardWidth/2,-nCardHeight/2);
//        painter.drawText(QRectF(MM2Pixel(var->fxPos), MM2Pixel(var->fyPos), TextRect.width(), TextRect.height()), Qt::AlignLeft | Qt::AlignTop, imageText);
//        int nTop = (int)roundf(MM2Pixel(var->fxPos));
//        int nLeft = (int)roundf(MM2Pixel(var->fyPos));

//        SetDarkTextRegion(nLeft,nTop, nLeft + TextRect.width(),nTop + TextRect.height());

//        painter.restore();
//    }
//    if (strPreviewFile.size())
//        Convas.save(strPreviewFile.c_str());
//    //Convas.save("/storage/emulated/0/MagazineUnlock/PrintSample.bmp");
//    // /storage/emulated/0/MagazineUnlock

//    char *pFileBuffer = nullptr;
//    int nBufferSize = Convert_dib(&pFileBuffer,Convas);
//    if (!nBufferSize)
//    {
//        if (pFileBuffer)
//            delete []pFileBuffer;
//        RunlogF("failed in  Convert_dib:%s.\n");
//        return 1;
//    }

//    if (strGRibbonType.size())
//    {
//        RunlogF("Try to evolis_print_set_option(GRibbonType,%s).\n",strGRibbonType.c_str());
//        if (evolis_print_set_option(m_pPrinter, "GRibbonType", strGRibbonType.c_str()))
//        {
//            RunlogF("Failed in evolis_print_set_option(GRibbonType,%s).\n",strGRibbonType.c_str());
//            return 1;
//        }
//        RunlogF("evolis_print_set_option(GRibbonType,%s) Succeed.\n",strGRibbonType.c_str());
//    }

////    if (strIFDarkLevelValue.size())
////    {
////        RunlogF("Try to evolis_print_set_option(IFDarkLevelValue,%s).\n",strIFDarkLevelValue.c_str());
////        if (evolis_print_set_option(m_pPrinter, "IFDarkLevelValue", strIFDarkLevelValue.c_str()))
////        {
////            RunlogF("Failed in evolis_print_set_option(IFDarkLevelValue,%s).\n",strIFDarkLevelValue.c_str());
////            return 1;
////        }
////        RunlogF("evolis_print_set_option(IFDarkLevelValue,%s) Succeed.\n",strIFDarkLevelValue.c_str());
////    }

//    strOverlayer = "/mnt/internal_sd/Z390/iso.bmp";
//    QFileInfo fi(strOverlayer.c_str());
//    if (!fi.isFile())
//    {
//        RunlogF("Can't open file %s.\n",strOverlayer.c_str());
//        return 1;
//    }
//    RunlogF("Try to evolis_print_set_option(FOverlayManagement,BMPVARNISH).\n");
//    if (evolis_print_set_option(m_pPrinter,"FOverlayManagement", "BMPVARNISH"))
//    {
//        RunlogF("Failed in evolis_print_set_option(FOverlayManagement,BMPVARNISH).\n");
//        return 1;
//    }
//    RunlogF("evolis_print_set_option(FOverlayManagement,BMPVARNISH) Succeed.\n");

//    RunlogF("Try to evolis_print_set_option(IFOverlayCustom,%s).\n",strOverlayer.c_str());
//    if (evolis_print_set_option(m_pPrinter,"IFOverlayCustom",strOverlayer.c_str()))
//    {
//        RunlogF("Failed in evolis_print_set_option(FOverlayManagement,BMPVARNISH).\n",strOverlayer.c_str());
//        return 1;
//    }
//    RunlogF("evolis_print_set_option(IFOverlayCustom,%s) Succeed.\n",strOverlayer.c_str());

//    if (evolis_print_set_imageb(m_pPrinter, evolis_face_t::EVOLIS_FA_FRONT, pFileBuffer,nBufferSize))
//    {
//        RunlogF("evolis_print_set_imageb failed:%s.\n",inPicInfo.picpath.c_str());
//        return 1;
//    }

//    RunlogF("Try to evolis_print_exec.\n");
//    evolis_print_exec((evolis_t*)m_pPrinter);
//    int             printed = 0;
//    evolis_status_t es;
//    auto tNow = system_clock::now();
//    auto tLast = tNow;
//    while (!printed)
//    {
//        if (evolis_status(m_pPrinter, &es) != 0)
//        {
//            RunlogF("Error reading printer status\n");
//            return 0;
//        }
//        printed = !(es.information & EVOLIS_INF_BUSY);
//        tNow = system_clock::now();
//        auto tDeration = duration_cast<milliseconds>(tNow - tLast);
//        if (tDeration.count() > 500)
//        {
//            tLast = tNow;
//            RunlogF("Waiting printer task stop\n");
//        }
//    }
//    return 0;
//}

Mat matRotateClockWise180(Mat src)//顺时针180
{
    if (src.empty())
    {
        qDebug() << "RorateMat src is empty!";
    }

    //0: 沿X轴翻转； >0: 沿Y轴翻转； <0: 沿X轴和Y轴翻转
    flip(src, src, 0);// 翻转模式，flipCode == 0垂直翻转（沿X轴翻转），flipCode>0水平翻转（沿Y轴翻转），flipCode<0水平垂直翻转（先沿X轴翻转，再沿Y轴翻转，等价于旋转180°）
    flip(src, src, 1);
    return src;
    //transpose(src, src);// 矩阵转置
}


int QEvolisPrinter::PrintCard(PICINFO& inPicInfo, vector<TextInfoPtr>& inTextVector)
{
    Funclog();
    if (!m_pPrinter)
        return 1;

    QFileInfo fipic(inPicInfo.picpath.c_str());
    if (!fipic.isFile() && !inTextVector.size())
    {
        RunlogF("There is no any content to print!\n")
            return 1;
    }

    RunlogF("Try to evolis_print_init.\n");
    if (evolis_print_init((evolis_t*)m_pPrinter))
    {
        RunlogF("evolis_print_init failed.\n");
        return 1;
    }
    RunlogF("evolis_print_init Succeed.\n");


    Mat canvas(nCardHeight,nCardWidth,CV_8UC3,Scalar(255,255,255));

    RunlogF("Try to load file %s.\n",inPicInfo.picpath.c_str());

    Mat HeaderImage = imread(inPicInfo.picpath, IMREAD_ANYCOLOR);
    if (!HeaderImage.data)
    {
         RunlogF("Failed in load file :%s.\n",inPicInfo.picpath.c_str());
         return 1;
    }
    Mat HeaderPrint;
    resize(HeaderImage, HeaderPrint, Size(MM2Pixel(inPicInfo.fWidth) , MM2Pixel(inPicInfo.fHeight)), 0, 0, INTER_NEAREST);

    Mat HeaderROI = canvas(Rect(MM2Pixel(inPicInfo.fxPos),MM2Pixel(inPicInfo.fyPos),MM2Pixel(inPicInfo.fWidth),MM2Pixel(inPicInfo.fHeight)));
    HeaderPrint.copyTo(HeaderROI);

    for (auto var : inTextVector)
    {
        RunlogF("Text = %s.\n",var->sText.c_str());
        Mat FontROI = canvas(Rect(10, 280, 1000, 300));
        IplImage FontImag= cvIplImage(FontROI);
        CVText_CN cvTextCn("/mnt/internal_sd/Z390/simsun.ttc", MM2Pixel(var->nFontSize));

        wchar_t szUnicode[1024] = {0};
        mbstowcs(szUnicode,var->sText.c_str(),var->sText.size());
        cvTextCn.putText(&FontImag, szUnicode, cvPoint(MM2Pixel(var->nFontSize), MM2Pixel(var->nFontSize)), cvScalar(CV_RGB(0, 0, 0)));
    }

    if (strPreviewFile.size())
        imwrite(strPreviewFile.c_str(),canvas);
    const char *szTempFile = "/mnt/internal_sd/Z390/PrintPreview.bmp";
    if (inPicInfo.nAngle > 0)
    {
        //0: 沿X轴翻转； >0: 沿Y轴翻转； <0: 沿X轴和Y轴翻转
        flip(canvas, canvas, 0);// 翻转模式，flipCode == 0垂直翻转（沿X轴翻转），flipCode>0水平翻转（沿Y轴翻转），flipCode<0水平垂直翻转（先沿X轴翻转，再沿Y轴翻转，等价于旋转180°）
        flip(canvas, canvas, 1);
    }
    imwrite(szTempFile,canvas);

    char *pFileBuffer = nullptr;
    int nBufferSize = 0;
    //Convert_dib(&pFileBuffer,Convas);
//    if (!nBufferSize)
//    {
//        if (pFileBuffer)
//            delete []pFileBuffer;
//        RunlogF("failed in  Convert_dib:%s.\n");
//        return 1;
//    }
    QFile filepic(szTempFile);
    if (!filepic.open(QIODevice::ReadOnly))
    {
        RunlogF("Filed to in open file:%s.\n",szTempFile);
        return 1;
    }
    nBufferSize = filepic.size();
    if (!nBufferSize)
    {
        RunlogF("File %s is Zero size.\n",szTempFile);
        return 1;
    }
    pFileBuffer = new char [filepic.size()];
    if (!pFileBuffer)
    {
        RunlogF("Insufficent memory in reading file :%s!\n",szTempFile);
        return 1;
    }

    shared_ptr<char> AutoBuffer(pFileBuffer);
    filepic.read(pFileBuffer,nBufferSize);
    if (strGRibbonType.size())
    {
        RunlogF("Try to evolis_print_set_option(GRibbonType,%s).\n",strGRibbonType.c_str());
        if (evolis_print_set_option(m_pPrinter, "GRibbonType", strGRibbonType.c_str()))
        {
            RunlogF("Failed in evolis_print_set_option(GRibbonType,%s).\n",strGRibbonType.c_str());
            return 1;
        }
        RunlogF("evolis_print_set_option(GRibbonType,%s) Succeed.\n",strGRibbonType.c_str());
    }

    //    if (strIFDarkLevelValue.size())
    //    {
    //        RunlogF("Try to evolis_print_set_option(IFDarkLevelValue,%s).\n",strIFDarkLevelValue.c_str());
    //        if (evolis_print_set_option(m_pPrinter, "IFDarkLevelValue", strIFDarkLevelValue.c_str()))
    //        {
    //            RunlogF("Failed in evolis_print_set_option(IFDarkLevelValue,%s).\n",strIFDarkLevelValue.c_str());
    //            return 1;
    //        }
    //        RunlogF("evolis_print_set_option(IFDarkLevelValue,%s) Succeed.\n",strIFDarkLevelValue.c_str());
    //    }

    strOverlayer = "/mnt/internal_sd/Z390/iso.bmp";
    QFileInfo fi(strOverlayer.c_str());
    if (!fi.isFile())
    {
        RunlogF("Can't open file %s.\n",strOverlayer.c_str());
        return 1;
    }
    RunlogF("Try to evolis_print_set_option(FOverlayManagement,BMPVARNISH).\n");
    if (evolis_print_set_option(m_pPrinter,"FOverlayManagement", "BMPVARNISH"))
    {
        RunlogF("Failed in evolis_print_set_option(FOverlayManagement,BMPVARNISH).\n");
        return 1;
    }
    RunlogF("evolis_print_set_option(FOverlayManagement,BMPVARNISH) Succeed.\n");

    RunlogF("Try to evolis_print_set_option(IFOverlayCustom,%s).\n",strOverlayer.c_str());
    if (evolis_print_set_option(m_pPrinter,"IFOverlayCustom",strOverlayer.c_str()))
    {
        RunlogF("Failed in evolis_print_set_option(FOverlayManagement,BMPVARNISH).\n",strOverlayer.c_str());
        return 1;
    }
    RunlogF("evolis_print_set_option(IFOverlayCustom,%s) Succeed.\n",strOverlayer.c_str());

    if (evolis_print_set_imageb(m_pPrinter, evolis_face_t::EVOLIS_FA_FRONT, pFileBuffer,nBufferSize))
    {
        RunlogF("evolis_print_set_imageb failed:%s.\n",inPicInfo.picpath.c_str());
        return 1;
    }

    RunlogF("Try to evolis_print_exec.\n");
    evolis_print_exec((evolis_t*)m_pPrinter);
    int             printed = 0;
    evolis_status_t es;
    auto tNow = system_clock::now();
    auto tLast = tNow;
    while (!printed)
    {
        if (evolis_status(m_pPrinter, &es) != 0)
        {
            RunlogF("Error reading printer status\n");
            return 0;
        }
        printed = !(es.information & EVOLIS_INF_BUSY);
        tNow = system_clock::now();
        auto tDeration = duration_cast<milliseconds>(tNow - tLast);
        if (tDeration.count() > 500)
        {
            tLast = tNow;
            RunlogF("Waiting printer task stop\n");
        }
    }
    return 0;
}
