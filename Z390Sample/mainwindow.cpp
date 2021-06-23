#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFontDialog>
#include <qfiledialog.h>

#include <string>
#include <QtDebug>
#include <QStandardPaths>
#include <memory>
#include <QDir>
#include <QList>
#include <QFileInfoList>
#include <QDebug>
#include <android/log.h>
#include <string>
#include <QColorDialog>
#include <QEvent>
#include <QMouseEvent>
#include "./Z390/include/evolis.h"
#include "./Z390/include/android.h""
#include "./Z390/include/evo-printers.h"



//#include "./Z390/include/evolis_z390_lithographprinter.h"

using namespace std;

#define GetPrinter(x)     (Z390Ptr)x->Printer();
#define CheckPriner(x)     if (!x)    {    OutputMsg("Please Open printer!\n");    return;}

#define CreatePrinter(x)     AutoPrinter PrintInstance; CLithographPrinter* x = PrintInstance.Printer();
#define Result(x,f)  x = f; if (x < 0) {OutputMsg("Failed in %s:%d\n",#f,x);return;}else OutputMsg("Succeed in %s\n",#f);

struct ExtraCommand
{
    QString strCommand;
    QStringList listData;
};
ExtraCommand cmdlist[] =
    {
        {"ReadBankNo",{}},
        {"WriteCard:1",{}},
        {"WriteCard:2",{"RESULT1","RESULT2"}},
        {"WriteCard:3",{"RESULT","SHBZHM","XM","XB","MZ","CSRQ"}},
        {"WriteCard:4",{"KSBM","KLB","GFBB","CSHJGBM","FKRQ","KYXQ","KH","RESULT"}},
        {"WriteCard:5",{"RESULT1","RESULT2"}},
        {"WriteCard:6",{"<ROOT><USERPIN>123456</USERPIN></ROOT>"}},
        {"WriteCard:7",{"USERPIN","QMZS","RESULT","JMZS","JMMY","OLDGLYPIN","GLYPIN","OLDZKMY","ZKMY"}},
   };
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //setOrientation(SCREEN_ORIENTATION_LANDSCAPE);
    ui->setupUi(this);
    for(auto var:cmdlist)
    {
        ui->comboBox_Extracommand->addItem(var.strCommand);
    }
    ui->comboBox_Extracommand->setCurrentIndex(0);
   //pConvas = new QImage(1024, 650, QImage::Format_ARGB32);
   //pConvas->fill(Qt::white);
    //ui->label_Image->setPixmap(QPixmap::fromImage(*pConvas));
    ui->lineEdit_text_Color->installEventFilter(this);
    pAppPerissions = new QAndroidAppPermissions();
    QStringList strRequestPermissions = { "android.permission.READ_EXTERNAL_STORAGE",
                                          "android.permission.WRITE_EXTERNAL_STORAGE"};
    connect(pAppPerissions,&QAndroidAppPermissions::requestPermissionsResults,this,&MainWindow::OnRequestPermissionsResults);
    pAppPerissions->requestPermissions(strRequestPermissions);

}

void MainWindow::OnRequestPermissionsResults(const QVariantList &results)
{
    for (auto var:results)
    {
        QVariantMap &permissionResult = (QVariantMap &)var;
        QString strPermisstionItem = permissionResult["name"].toString();
        bool bResult = permissionResult["granted"].toBool();
        OutputMsg("Permission %s Request result:%s.\n",strPermisstionItem.toStdString().c_str(),bResult?"true":"false");
    }
}

void MainWindow::on_comboBox_Extracommand_currentIndexChanged(int index)
{
    if (index != -1)
    {
        ui->comboBox_ExtracommandData->clear();
        auto var = cmdlist[index];
        ui->comboBox_ExtracommandData->addItems(var.listData);
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}

#define __countof(array) (sizeof(array)/sizeof(array[0]))
void MainWindow::OutputMsg(const char *pFormat, ...)
{
    va_list args;
    va_start(args, pFormat);
    int nBuff;
    CHAR szBuffer[8192] = {0};
    nBuff = vsnprintf(szBuffer, __countof(szBuffer), pFormat, args);
    __android_log_write(ANDROID_LOG_INFO,"Debug",szBuffer);
    ui->textEdit->insertPlainText(szBuffer);
    va_end(args);
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(cursor);
}

void MainWindow::OpenPrinter()
{
    if (!pAutoPrinter)
    {
        pAutoPrinter = new AutoPrinter();
        pPrinter = GetPrinter(pAutoPrinter);
        if (!pPrinter)
        {
            OutputMsg("Failed in Opening Printer!\n");
            delete pAutoPrinter;
            pAutoPrinter = nullptr;
            return;
        };
        char szRCode[1024] = {0};
        int x = 0;
        Result(x,pPrinter->Print_Open("","",szRCode));
    }
    else
    {
        OutputMsg("Printer has been Opened!\n");
        return;
    }
}

void MainWindow::Printer_SetOverlayer()
{
    CheckPriner(pPrinter);

    char szRCode[1024] = {0};
    long lTimeout = 2000;
    char szCommandout[128] = {0};

    if (pPrinter->Print_ExtraCommand(lTimeout,"Set Overlayer",(void *)"assets:/overlayer.bmp",(LPVOID &)szCommandout,szRCode))
    {
        OutputMsg("Print_GetBoxStatus Failed!\n");
    }
}

void MainWindow::ClosePrinter()
{
    if (pAutoPrinter)
    {
        char szRCode[1024] = {0};
        pPrinter->Print_Close(szRCode);
        pPrinter = nullptr;
        pAutoPrinter->pPrinter = nullptr;
        delete pAutoPrinter;
        pAutoPrinter = nullptr;
    }
}
void MainWindow::Printer_GetBoxStatus()
{
    CheckPriner(pPrinter);

    char szRCode[1024] = {0};

    long lTimeout = 2000;
    Lithograph::LPLITHOGRAPHBOXINFO lpBoxInfo = new Lithograph::LITHOGRAPHBOXINFO;
    if (pPrinter->Print_GetBoxStatus(lTimeout,lpBoxInfo,szRCode))
    {
        OutputMsg("Print_GetBoxStatus Failed!\n");
    }
    else
    {
        const char *szBoxType[] = {"未知","发卡箱","回收箱"};
        const char *szBoxStatus[] = {"正常", "卡少","无卡", "不可操作", "不存在", "高(快满)","满","未知"};
        const char *szSense[] = {"否","是"};

        OutputMsg("Print_GetBoxStatus Succeed,Boxes Count:%d\n",lpBoxInfo->nCount);
        OutputMsg("Card boxes status:\n");
//        OutputMsg("卡箱号\t类型\t状态\t支持状态传感器\n");
        OutputMsg("卡箱号\t");
        OutputMsg("类型\t");
        OutputMsg("状态\t");
        OutputMsg("支持状态传感器\n");
        for(int i = 0;i < lpBoxInfo->nCount;i++)
        {
            OutputMsg("%d\t",lpBoxInfo->lpplist[i].byBoxNumber);
            OutputMsg("%s\t",szBoxType[lpBoxInfo->lpplist[i].byType]);
            OutputMsg("%s\t",szBoxStatus[lpBoxInfo->lpplist[i].byStatus]);
            OutputMsg("%s\n",szSense[lpBoxInfo->lpplist[i].bHardwareSensor]);
        }
    }
}

void MainWindow::Printer_InsertCard()
{
    CheckPriner(pPrinter);

    char szRCode[1024] = {0};

    long lTimeout = 2000;

    if (pPrinter->Print_Dispense(lTimeout,0,1,szRCode))
        OutputMsg("Failed in Print_Dispense:%s\n",szRCode);
    else
        OutputMsg("Print_Dispense Succeed.\n");
}

void MainWindow::Printer_EjectCard()
{
    CheckPriner(pPrinter);

    char szRCode[1024] = {0};
    long lTimeout = 2000;
    if (pPrinter->Print_Eject(lTimeout,szRCode))
        OutputMsg("Failed in Print_Eject:%s",szRCode);
    else
        OutputMsg("Print_Eject Succeed.\n",szRCode);
}

 void MainWindow::Printer_Retract()
 {
     CheckPriner(pPrinter);

     char szRCode[1024] = {0};
     long lTimeout = 2000;
     if (pPrinter->Print_Retract(lTimeout,0,szRCode))
         OutputMsg("Failed in Print_Retract:%s",szRCode);
     else
         OutputMsg("Print_Retract Succeed.\n",szRCode);
 }

void MainWindow::Printer_GetStatus()
{
    CheckPriner(pPrinter);
    long lTimeout = 2000;
    char szRCode[1024] = {0};
    Lithograph::LPLITHOGRAPHSTATUS lpStatus = new Lithograph::LITHOGRAPHSTATUS;

    if (pPrinter->Print_Status(lTimeout,lpStatus,szRCode))
    {
        OutputMsg("Print_Status Failed!\n");
        return ;
    }
    OutputMsg("Device = %d\tMedia = %d\tToner = %d.\n",lpStatus->fwDevice,(short)lpStatus->fwMedia,lpStatus->fwToner);
}

void MainWindow::Printer_Start()
{
    CheckPriner(pPrinter);
    long lTimeout = 2000;
    char szRCode[1024] = {0};
    if (ui->checkBox_SavePrintPage->isChecked())
    {
        LPVOID lpCmdOut = nullptr;

        pPrinter->Print_ExtraCommand(lTimeout,"Set SavePreview",(LPVOID)"/mnt/internal_sd/wltlib/PrintPriew.bmp",lpCmdOut,szRCode);
    }
    if (pPrinter->Print_StartPrint(lTimeout,szRCode))
    {
        OutputMsg("Print_StartPrint Failed!\n");
        return ;
    }
}

void MainWindow::Printer_Reset()
{
    CheckPriner(pPrinter);
    long lTimeout = 2000;
    char szRCode[1024] = {0};

    if (pPrinter->Print_Reset(lTimeout,nResetOption,szRCode))
    {
        OutputMsg("Print_Reset Failed!\n");
        return ;
    }
    else
    {
         OutputMsg("Print_Reset Succeed!\n");
    }
}

QString MainWindow::toAndroidPath(QString path)
{
    QAndroidJniObject uri;
    QString filename;
    uri = QAndroidJniObject::callStaticObjectMethod(
        "android/net/Uri", "parse", "(Ljava/lang/String;)Landroid/net/Uri;",
        QAndroidJniObject::fromString(path).object<jstring>());

    filename = QAndroidJniObject::callStaticObjectMethod(
                   "com/evolis/examples/PathUtil", "getFilePath",
                   "(Landroid/content/Context;Landroid/net/Uri;)Ljava/lang/String;",
                   QtAndroid::androidContext().object(), uri.object()).toString();

    return filename;
}

void MainWindow::Printer_ICPowerOn()
{
    CheckPriner(pPrinter);
    char szRCode[1024] = {0};
    int nArtlen = 0;
    long lTimeout = 2000;
    unsigned char szUID[1024] = {0};
    int nUidLen = 0;
    unsigned char szout[1024] = {0};
    if (pPrinter->Print_IcPowerOn(lTimeout,szout,nArtlen,(unsigned char *)szUID,nUidLen,szRCode))
    {
        OutputMsg("Print_IcPowerOn Failed!\n");
    }
    else
    {
        OutputMsg("Print_IcPowerOn Succeed!\n");
    }
}

void MainWindow::Printer_ICPowerOff()
{
    CheckPriner(pPrinter);
    char szRCode[1024] = {0};
    long lTimeout = 2000;
    if (pPrinter->Print_IcPowerOff(lTimeout,szRCode))
    {
        OutputMsg("Print_IcPowerOn Failed!\n");
    }
    else
    {
        OutputMsg("Print_IcPowerOn Succeed!\n");
    }
}

void MainWindow::Printer_ICExchange()
{
    CheckPriner(pPrinter);
    char szRCode[1024] = {0};
    long lTimeout = 2000;
    const char *szCmd = "0084000008";
    char szOut[1024] = {0};
    int nOutLen = 1024;
    if (pPrinter->Print_IcExchange(lTimeout,(unsigned char *)szCmd,10,(unsigned char *)szOut,nOutLen,szRCode))
        OutputMsg("Print_IcExchange Failed!\n");
    else
        OutputMsg("Print_IcExchange Succeed:%s!\n",szOut);
}


void MainWindow::Printer_Depense()
{
    CheckPriner(pPrinter);
    char szRCode[1024] = {0};

    long lTimeout = 2000;
    if (pPrinter->Print_Dispense(lTimeout,1,2,szRCode))
    {
        OutputMsg("Print_Dispense Failed!\n");
        return ;
    }
    else
        OutputMsg("Print_Dispense Succeed!\n");
}

void MainWindow::on_pushButton_ExtraCommand_clicked()
{
    CheckPriner(pPrinter);
    char szRCode[1024] = {0};
    int x;
    int nArtlen = 0;
    long lTimeout = 2000;
    unsigned char szUID[1024] = {0};
    int nUidLen = 0;
    unsigned char szout[1024] = {0};
    pPrinter->Print_IcPowerOff(lTimeout, szRCode);
    Result(x,pPrinter->Print_IcPowerOn(lTimeout,szout,nArtlen,(unsigned char *)szUID,nUidLen,szRCode));

    QString qstrCommand = ui->comboBox_Extracommand->currentText();
    QByteArray baCommand = qstrCommand.toLatin1();

    QString qstrData = ui->comboBox_ExtracommandData->currentText();
    QByteArray baData = qstrData.toLatin1();

    LPVOID szCommandout = nullptr;
    if (pPrinter->Print_ExtraCommand(lTimeout,baCommand.data(),baData.data(),szCommandout,szRCode))
    {
        OutputMsg("Print_ExtraCommand Failed!:%s\n",szRCode);
    }
    else
    {
        OutputMsg("Print_ExtraCommand Succeed:\n");
        QString strmsg = QString::fromLocal8Bit((char *)szCommandout);
        OutputMsg(strmsg.toStdString().c_str());
        OutputMsg("\n");
    }
    Result(x,pPrinter->Print_IcPowerOff(lTimeout, szRCode));
}

enum Printer_Func
{
    Open_Printer,
    Set_Overlayer,
    Close_Printer,
    CardBox_status,
    Insert_Card,
    Eject_Card,
    Retract,
    Printer_status,
    Start_Print,
    Depense,
    OpenReader
};

//void MainWindow::on_pushButton_Excute_clicked()
//{
//    Printer_Func nIndex = (Printer_Func)ui->comboBox_Function->currentIndex();

//    switch (nIndex)
//    {
//        case Open_Printer:

//            break;
//        case Set_Overlayer:
//            Printer_SetOverlayer();
//            break;
//        case Close_Printer:
//            ClosePrinter();
//            break;
//        case   CardBox_status:
//            Printer_GetBoxStatus();
//            break;
//        case   Insert_Card:
//            Printer_InsertCard();
//            break;
//        case   Eject_Card:
//            Printer_EjectCard();
//            break;
//        case Retract:
//            Printer_Retract();
//        case   Printer_status:
//            Printer_GetStatus();
//            break;
//        case   Start_Print:
//            Printer_Start();
//            break;
//        case   Depense:
//             Printer_Depense();
//            break;
//        case OpenReader:
//            Printer_OpenReader();
//            break;
//    }
//}

bool MainWindow::setOrientation(SCREEN_ORIENTATION orientation)
{
    const QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniEnvironment jniEnv;

    activity.callMethod<void>("setRequestedOrientation", "(I)V", orientation);

    if(jniEnv->ExceptionCheck())
    {
        jniEnv->ExceptionClear();
        return false;
    }
    return true;
}

void MainWindow::on_pushButton_browsegraph_clicked()
{
    const QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
    {
        QString strFile = toAndroidPath(fileName);
        qDebug()<<strFile;
        ui->lineEdit_graphpath->setText(strFile);

        QImage Convas(1024, 650, QImage::Format_RGB888);
        Convas.fill(Qt::white);
        QPainter painter(&Convas);
        painter.setCompositionMode(QPainter::CompositionMode_Source);

        QImage ImageLoad;
        painter.setRenderHint(QPainter::Antialiasing, true);
        float fAngle = ui->lineEdit_graph_angle->text().toFloat();

        /* 坐标转换公式
         * MM_w=Pixel_x*25.4/dpi
         * MM_h=Pixel_y*25.4/dpi
         * Pixel_x=MM_w*dpi/25.4
         * Pixel_y=MM_h*dpi/25.4
        */
        const double nDPI = 300;
        const double fScale = nDPI/25.4;
        if (ImageLoad.load(strFile))
        {
            double nX = ui->lineEdit_graphX->text().toDouble();
            double nY = ui->lineEdit_graphY->text().toDouble();
            double nW = ui->lineEdit_graphW->text().toDouble();
            double nH = ui->lineEdit_graphH->text().toDouble();

            double fPixel_x = nX*fScale;
            double fPixel_y = nY*fScale;
            double fW = nW*fScale;
            double fH = nH*fScale;

            painter.save();
            int nConvasWidth = Convas.width();
            int nConvasHeight = Convas.height();
            painter.translate(nConvasWidth/2,nConvasHeight/2);

            painter.rotate(fAngle);
            painter.translate(-nConvasWidth/2,-nConvasHeight/2);
            painter.drawImage(QRectF(fPixel_x,fPixel_y,fW,fH),ImageLoad);
            painter.restore();

            QFileInfo fi(strFile);
            QString strImageSave = fi.absoluteDir().absolutePath() + "/Sample.bmp";
            OutputMsg(strImageSave.toStdString().c_str());
            Convas.save(strImageSave);
        }
    }
}

void MainWindow::on_pushButton_Font_clicked()
{
    //setOrientation(SCREEN_ORIENTATION::SCREEN_ORIENTATION_REVERSE_LANDSCAPE);
}


bool MainWindow::eventFilter(QObject *obj, QEvent *e)
{
    if(obj == ui->lineEdit_text_Color && e->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *me = (QMouseEvent*)e;
        if(me->button() == Qt::LeftButton)
        {
            QColor color = QColorDialog::getColor(Qt::red,this, tr("Color Dialog"));
            QString strColor = QString::number((uint_t)color.rgb(),16);
            ui->lineEdit_text_Color->setText(strColor);
        }
    }
    return QMainWindow::eventFilter(obj,e);
}

//void MainWindow::on_pushButton_graph_Input_clicked()
//{
//}

//void MainWindow::on_pushButton_text_Input_clicked()
//{
//}

//void MainWindow::on_pushButton_ClearConvas_clicked()
//{
//     pConvas->fill(Qt::white);
//     ui->label_Image->setPixmap(QPixmap::fromImage(*pConvas));
//}

//void MainWindow::on_pushButton_LoadOverlayer_clicked()
//{
//    if (ui->checkBox_CheckPrinter->isChecked())
//    {
//        CheckPriner(pPrinter);
//    }
//    else
//    {
//        pPrinter = new Evolis_Z390_LithographPrinter;
//    }
//    QString strFile = "assets:/overlayer.bmp";
//    if (!ui->checkBox_InternalOverlay->isChecked())
//    {
//        const QString fileName = QFileDialog::getOpenFileName(this);
//        if (!fileName.isEmpty())
//        {
//            strFile = toAndroidPath(fileName);
//            qDebug()<<strFile;
//            long nTimeout = 2000;
//            ui->lineEdit_overlay->setText(strFile);
//        }
//    }
//    char OutCmd[128] = {0};
//    char szRcode[128] = {0};
//    if (pPrinter->Print_ExtraCommand(2000,"Set Overlayer",(LPVOID)strFile.toStdString().c_str(),(LPVOID&)OutCmd,szRcode))
//        OutputMsg("Print_ExtraCommand Failed!\n");
//    else
//        OutputMsg("%s has been set as overlayer!\n",strFile.toStdString().c_str());
//}

void MainWindow::on_pushButton_PrinterOpen_clicked()
{
    OpenPrinter();
}

void MainWindow::on_pushButton_PrinterInsert_clicked()
{
    Printer_InsertCard();
}

void MainWindow::on_pushButton_PrinterEject_clicked()
{
    Printer_EjectCard();
}

void MainWindow::on_pushButton_PrinterReject_clicked()
{
    Printer_Retract();
}

void MainWindow::on_pushButton_PrinterClose_clicked()
{
    ClosePrinter();
}

void MainWindow::on_pushButton_PrinterStatus_clicked()
{
    Printer_GetStatus();
}

void MainWindow::on_pushButton_PrinterBoxStatus_clicked()
{
    Printer_GetBoxStatus();
}

void MainWindow::on_pushButton_PrinterInit_clicked()
{
    CheckPriner(pPrinter);
    long lTimeout = 2000;
    char szRCode[1024] = {0};

    if (pPrinter->Print_InitPrint(lTimeout,szRCode))
    {
        OutputMsg("Print_InitPrint Failed!\n");
        return ;
    }
    else
         OutputMsg("Print_InitPrint Succeed!\n");
}

void MainWindow::on_pushButton_PrinterSetImage_clicked()
{
    //if (ui->checkBox_CheckPrinter->isChecked())
        CheckPriner(pPrinter);
    long lTimeout = 2000;
    char szRCode[1024] ={0};

    QString strFileImage = ui->lineEdit_graphpath->text();
    int nX = ui->lineEdit_graphX->text().toInt();
    int nY = ui->lineEdit_graphY->text().toInt();
    int nW = ui->lineEdit_graphW->text().toInt();
    int nH = ui->lineEdit_graphH->text().toInt();
    int nAngle = ui->lineEdit_graph_angle->text().toInt();

    QImage Convas(1024, 650, QImage::Format_RGB888);
    Convas.fill(Qt::white);
    QPainter painter(&Convas);

    painter.setCompositionMode(QPainter::CompositionMode_Source);

    QImage ImageLoad;
    painter.setRenderHint(QPainter::Antialiasing, true);

    if (ImageLoad.load(strFileImage))
    {
        painter.save();
        int nConvasWidth = Convas.width();
        int nConvasHeight = Convas.height();
        painter.translate(nConvasWidth/2,nConvasHeight/2);
        if (nAngle)
            painter.rotate(nAngle);
        painter.translate(-nConvasWidth/2,-nConvasHeight/2);
        painter.drawImage(QRectF(nX,nY,nH,nW),ImageLoad);
        painter.restore();
    }

//    if (ui->checkBox_CheckPrinter->isChecked())
//    {
        if (pPrinter->Print_PrintImage(lTimeout,(char *)strFileImage.toStdString().c_str(),nAngle,nX,nY,nH,nW,szRCode))
            OutputMsg("Print_PrintImage Failed!\n");
        else
            OutputMsg("Print_PrintImage Succeed!\n");
//    }
//    else
//        OutputMsg("Picture has input!\n");
}

void MainWindow::on_pushButton_PrinterSetText_clicked()
{
    //if (ui->checkBox_CheckPrinter->isChecked())
        CheckPriner(pPrinter);
    long lTimeout = 2000;

    char szRCode[1024] ={0};
    float x = ui->lineEdit_x->text().toFloat();
    float y = ui->lineEdit_y->text().toFloat();
    int nAngle = ui->lineEdit_text_angle->text().toInt();

    int size = ui->lineEdit_text_size->text().toInt();
    QString imageText = ui->lineEdit_text->text();

//    QPainter painter(pConvas);
//    /* 设置画刷的组合模式CompositionMode_SourceOut这个模式为目标图像在上。
//    改变组合模式和上面的填充方式可以画出透明的图片。*/
//    painter.setCompositionMode(QPainter::CompositionMode_Source);
//    //图片上的字符串，例如 HELLO
//    QFont font("宋体");
//    //设置显示字体的大小
//    font.setPixelSize(size);
//    QFontMetrics fm(font);
//    int charWidth = fm.horizontalAdvance(imageText);
//    auto TextRect = fm.boundingRect(imageText);
//    charWidth = fm.boundingRect(imageText).width();
//    painter.setFont(font);
//    QPen pen = painter.pen();
//    pen.setColor(QColor(0, 0, 0));
//    painter.setPen(pen);
//    int nConvasWidth = pConvas->width();
//    int nConvasHeight = pConvas->height();

//    painter.translate(nConvasWidth,nConvasHeight);
//    painter.save();
//    painter.rotate(nAngle);
//    painter.translate(-nConvasWidth,-nConvasHeight);
//    painter.drawText(QRectF(x, y, TextRect.width(), TextRect.height()), Qt::AlignLeft | Qt::AlignTop, imageText);

//    painter.restore();
////    ui->label_Image->setPixmap(QPixmap::fromImage(*pConvas));

////    if (ui->checkBox_CheckPrinter->isChecked())
////    {
      string strFont = "宋体";
      int nFontSize = 8;
      pPrinter->Print_PrintText(lTimeout,"姓名 测试用户",nAngle,26,14,"宋体",size,1,0,szRCode);
      pPrinter->Print_PrintText(lTimeout,"社会保障号码  123456789012345678",nAngle,26,19,"宋体",size,1,0,szRCode);
      pPrinter->Print_PrintText(lTimeout,"社会保障卡号  ABCDEFGHIJKLMN",nAngle,26,24,"宋体",size,1,0,szRCode);
      pPrinter->Print_PrintText(lTimeout,"发卡日期  2019年9月27日",nAngle,26,29,"宋体",size,1,0,szRCode);
//        if (pPrinter->Print_PrintText(lTimeout,(char *)imageText.toStdString().c_str(),nAngle,x,y,"宋体",size,1,0,szRCode))
//            OutputMsg("Print_PrintText Failed!\n");
//        else
            OutputMsg("Print_PrintText Succeed!\n");
////    }
////    else
////        OutputMsg("Text has input!\n");
}

void MainWindow::on_pushButton_PrinterICOn_clicked()
{
    Printer_ICPowerOn();
}

void MainWindow::on_pushButton_PrinterICExchange_clicked()
{
    Printer_ICExchange();
}

void MainWindow::on_pushButton_PrinterICOff_clicked()
{
    Printer_ICPowerOff();
}

void MainWindow::on_pushButton_PrinterStart_clicked()
{
     Printer_Start();
}

void MainWindow::on_pushButton_PrinterReset_clicked()
{
    Printer_Reset();
}

void MainWindow::on_radioButton_Keep_clicked()
{
    nResetOption = 1;
}

void MainWindow::on_radioButton_Eject_clicked()
{
    nResetOption = 2;
}

void MainWindow::on_radioButton_Reject_clicked()
{
    nResetOption = 3;
}

void MainWindow::on_pushButton_PrinterConfigure_Get_clicked()
{

}

void MainWindow::on_pushButton_PrinterConfigure_Set_clicked()
{
//    CheckPriner(pPrinter);
//    char szRCode[1024] = {0};
//    int x;

//    long lTimeout = 2000;

//    QString strRibionType = ui->comboBox_GRibbontype->currentText();
//    QString strIFDarkLevelValue = ui->lineEdit_IFDraklevelValue->text();

//    LPVOID szCommandout = nullptr;
//    if (pPrinter->Print_ExtraCommand(lTimeout,(char *)"Set GRibbonType",(LPVOID)strRibionType.toStdString().c_str(),szCommandout,szRCode))
//    {
//        OutputMsg("Print_ExtraCommand(Set GRibbonType) Failed!:%s\n",szRCode);
//    }
//    else
//    {
//        OutputMsg("Print_ExtraCommand(Set GRibbonType) Succeed.\n");
//    }

//    if (pPrinter->Print_ExtraCommand(lTimeout,(char *)"Set IFDarkLevelValue",(LPVOID)strIFDarkLevelValue.toStdString().c_str(),szCommandout,szRCode))
//    {
//        OutputMsg("Print_ExtraCommand(Set IFDarkLevelValue) Failed!:%s\n",szRCode);
//    }
//    else
//    {
//        OutputMsg("Print_ExtraCommand(Set IFDarkLevelValue) Succeed.\n");
//    }
    evolis_t*   printer;
    QAndroidJniEnvironment jniEnv;
    QAndroidJniObject androidCtx = QtAndroid::androidContext();
    evolis_set_android_env(jniEnv, androidCtx.object());
    if ((printer = evolis_open2("usb:///dev/usb/lp0", EVOLIS_TY_EVOLIS)) != NULL)
    {
        evolis_print_init(printer);
        evolis_print_set_option(printer, "GRibbonType", "RC_YMCKO");
        evolis_print_set_option(printer, "FBlackManagement", "TEXTINBLACK");
        evolis_print_set_option(printer, "IFTextRegion", "0x0x1000x220");
        evolis_print_set_option(printer, "IFBlackLevelValue", "40");
        evolis_print_set_option(printer, "IFDarkLevelValue ", "10");
        evolis_print_set_imagep(printer, EVOLIS_FA_FRONT, "/mnt/internal_sd/Z390/PrintPreview.bmp");
        evolis_print_exec(printer);
        evolis_close(printer);
    }
    else
    {
        OutputMsg("Error: %s - %s\n", evolis_last_error_string(), strerror(errno));
    }
}
