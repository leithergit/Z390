#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMessageBox>
#include <stdio.h>
#include <QImage>
#include <QtDebug>
#include <dlfcn.h>
#include <QAndroidJniObject>
#include <QObject>
#include <QtAndroid>
#include <QtAndroidExtras>
#include <QAndroidJniEnvironment>
#include "QAndroidAppPermissions.h"
#include "./Z390/include/LithographPrinter.h"
#include <QApplication>
#include <thread>
#include <chrono>
using namespace std;

class QWaitCursor
{
public:
    QWaitCursor()
    {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    }
    ~QWaitCursor()
    {
        QApplication::restoreOverrideCursor();
    }
};

#define WaitCursor()  QWaitCursor qWait;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using pCreateInstance =  LPVOID  (*)(LPVOID lpReserve);
using pFreeInstance = void (*)(LPVOID lpDev);

using Z390Ptr = CLithographPrinter*;

enum SCREEN_ORIENTATION
{
    SCREEN_ORIENTATION_LANDSCAPE = 0,
    SCREEN_ORIENTATION_REVERSE_LANDSCAPE = 8,
    SCREEN_ORIENTATION_SENSOR_LANDSCAPE = 6,
    SCREEN_ORIENTATION_PORTRAIT = 1,
    SCREEN_ORIENTATION_REVERSE_PORTRAIT = 9,
    SCREEN_ORIENTATION_SENSOR_PORTRAIT = 7
};

class AutoPrinter
{
public:
    CLithographPrinter* pPrinter = nullptr;
    void *pLibhandle = nullptr;
    pCreateInstance pCreateInst = nullptr;
    pFreeInstance pFreeInst = nullptr;
    char szError[1024];
    AutoPrinter()
    {
        memset(szError,0,1024);
        pLibhandle = dlopen("libEvolis_Z390_LithographPrinter_armeabi-v7a.so",RTLD_NOW);
        if (pLibhandle)
        {
            pCreateInst = (pCreateInstance) dlsym(pLibhandle,"CreateInstance");
            pFreeInst = (pFreeInstance)dlsym(pLibhandle,"FreeInstance");
            if (!pCreateInst || !pFreeInst)
            {
                strcpy(szError,"Failed in loading Function CreateInstance!");
                qDebug()<<szError;
                return ;
            }
            pPrinter = (CLithographPrinter*)pCreateInst(nullptr);
            if (!pPrinter)
            {
                strcpy(szError,"Failed in Creating Evolis_Z390_LithographPrinter Instance!");
                qDebug()<<szError;
                return;
            }
        }
        else
        {
            strcpy(szError,"Failed in loading libEvolis_Z390_LithographPrinter_armeabi-v7a.so!");
            qDebug()<<szError;
        }
    }
    ~AutoPrinter()
    {
        if (pPrinter)
        {
            char szCode[1024] = {0};
            pPrinter->Print_Close(szCode);
            pFreeInst(pPrinter);
            pPrinter = nullptr;
        }
        if (pLibhandle)
            dlclose(pLibhandle);
    }

    CLithographPrinter *Printer()
    {
        return pPrinter;
    }

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void OutputMsg(const char *pFormat, ...);
    void *pPrinterInstance = nullptr;
    //QImage *pConvas = nullptr;
    void OpenPrinter();
    void Printer_SetOverlayer();
    void ClosePrinter();
    AutoPrinter *pAutoPrinter = nullptr;
    Z390Ptr pPrinter = nullptr;
    QString toAndroidPath(QString path);
    std::thread *pThreadPrint = nullptr;
    int nJobs = 3;

    Q_INVOKABLE bool setOrientation(SCREEN_ORIENTATION orientation);

    void Printer_GetBoxStatus();

    void Printer_InsertCard();

    void Printer_EjectCard();

    void Printer_Retract();

    void Printer_GetStatus();

    void Printer_Start();

    void Printer_Reset();

    void Printer_ICPowerOn();

    void Printer_ICPowerOff();

    void Printer_ICExchange(const char *szCmd);

    void Printer_Depense();
    void PrintJos();

    bool GetCardBoxStatus();
protected:
    bool eventFilter(QObject *obj, QEvent *e);
signals:
    void RestoreButtons();
    void AppendText(QString strTxt);
    void ScrollToEnd();


private slots:

    void OnRestoreButtons();
    void OnAppendText(QString strText);
    void OnScrollToEnd();

    void on_pushButton_ExtraCommand_clicked();

    void on_comboBox_Extracommand_currentIndexChanged(int index);

    //void on_pushButton_Excute_clicked();

    void on_pushButton_browsegraph_clicked();

    void on_pushButton_Font_clicked();

    void OnRequestPermissionsResults(const QVariantList &results);

    //void on_pushButton_graph_Input_clicked();

    //void on_pushButton_text_Input_clicked();

    //void on_pushButton_ClearConvas_clicked();

    //void on_pushButton_LoadOverlayer_clicked();

    void on_pushButton_PrinterOpen_clicked();

    void on_pushButton_PrinterInsert_clicked();

    void on_pushButton_PrinterEject_clicked();

    void on_pushButton_PrinterReject_clicked();

    void on_pushButton_PrinterClose_clicked();

    void on_pushButton_PrinterStatus_clicked();

    void on_pushButton_PrinterBoxStatus_clicked();

    void on_pushButton_PrinterInit_clicked();

    void on_pushButton_PrinterSetImage_clicked();

    void on_pushButton_PrinterSetText_clicked();

    void on_pushButton_PrinterICOn_clicked();

    void on_pushButton_PrinterICExchange_clicked();

    void on_pushButton_PrinterICOff_clicked();

    void on_pushButton_PrinterStart_clicked();

    void on_pushButton_PrinterReset_clicked();

    void on_radioButton_Keep_clicked();

    void on_radioButton_Eject_clicked();

    void on_radioButton_Reject_clicked();

    void on_pushButton_PrinterConfigure_Get_clicked();

    void on_pushButton_PrinterConfigure_Set_clicked();

    void on_pushButton_PrinterJob_clicked();

private:
    Ui::MainWindow *ui;
    int  nResetOption = 1;
    QAndroidAppPermissions *pAppPerissions = nullptr;
};
#endif // MAINWINDOW_H
