#include "mainwindow.h"
#include <QApplication>
//#include "./Z390/include/evolis_z390_lithographprinter.h"
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QtAndroid>
#include <jni.h>
#include <QScreen>
#include <QDebug>
#include <android/log.h>

int main(int argc, char *argv[])
{
    for (int i = 0;i < argc ;i ++)
    {
        __android_log_print(ANDROID_LOG_INFO,"Debug","argv[%d] = %s.\n",i,argv[i]);
    }
    QString strQtPluginpath = qEnvironmentVariable("QT_QPA_PLATFORM_PLUGIN_PATH");
    __android_log_print(ANDROID_LOG_INFO,"Debug","QT_QPA_PLATFORM_PLUGIN_PATH = %s.\n",strQtPluginpath.toStdString().c_str());
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QAndroidJniEnvironment jniEnv;
    QAndroidJniObject androidCtx = QtAndroid::androidContext();
//    jobject jobj = androidCtx.object();
//    JavaVM* JVM;
//    jniEnv->GetJavaVM(&JVM);
    //JVM->GetEnv()
    //evolis_set_android_env(jniEnv, androidCtx.object());
    //InitZ390(jniEnv,androidCtx.object());

    //要加#include <QScreen>和#include <QApplication>
    auto  pScreen = qApp->primaryScreen();
    if (pScreen)//要设置需要的屏幕方向，才能收到QScreen的orientationChanged信号
        pScreen->setOrientationUpdateMask(Qt::PortraitOrientation | Qt::LandscapeOrientation| Qt::InvertedLandscapeOrientation);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
