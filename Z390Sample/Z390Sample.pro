QT       += core gui
android {
QT += androidextras

}


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    QAndroidAppPermissions.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    QAndroidAppPermissions.h \
    Z390/include/Evolis_Z390_LithographPrinter_global.h \
    Z390/include/LithographPrinter.h \
    Z390/include/android.h \
    Z390/include/ava-printers.h \
    Z390/include/decard/BasicOper.h \
    Z390/include/decard/dcrf32.h \
    Z390/include/evo-printers.h \
    Z390/include/evolis.h \
    Z390/include/evolis_z390_lithographprinter.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$PWD/Z390/lib/armeabi-v7a/ -ldc_mobile_mate

INCLUDEPATH += $$PWD/Z390/lib/armeabi-v7a
DEPENDPATH += $$PWD/Z390/lib/armeabi-v7a

unix:!macx: LIBS += -L$$PWD/Z390/lib/armeabi-v7a/ -ldcrf32

INCLUDEPATH += $$PWD/Z390/lib/armeabi-v7a
DEPENDPATH += $$PWD/Z390/lib/armeabi-v7a

unix:!macx: LIBS += -L$$PWD/Z390/lib/armeabi-v7a/ -levolis

INCLUDEPATH += $$PWD/Z390/lib/armeabi-v7a
DEPENDPATH += $$PWD/Z390/lib/armeabi-v7a

unix:!macx: LIBS += -L$$PWD/Z390/lib/armeabi-v7a/ -lwlt2bmp

INCLUDEPATH += $$PWD/Z390/lib/armeabi-v7a
DEPENDPATH += $$PWD/Z390/lib/armeabi-v7a

unix:!macx: LIBS += -L$$PWD/Z390/lib/armeabi-v7a/ -lEvolis_Z390_LithographPrinter_armeabi-v7a
INCLUDEPATH += $$PWD/Z390/lib/armeabi-v7a
DEPENDPATH += $$PWD/Z390/lib/armeabi-v7a

DISTFILES += \
    OpenCV/3rdparty.libs/armeabi-v7a/libIlmImf.a \
    OpenCV/3rdparty.libs/armeabi-v7a/libade.a \
    OpenCV/3rdparty.libs/armeabi-v7a/libcpufeatures.a \
    OpenCV/3rdparty.libs/armeabi-v7a/libittnotify.a \
    OpenCV/3rdparty.libs/armeabi-v7a/liblibjpeg-turbo.a \
    OpenCV/3rdparty.libs/armeabi-v7a/liblibopenjp2.a \
    OpenCV/3rdparty.libs/armeabi-v7a/liblibpng.a \
    OpenCV/3rdparty.libs/armeabi-v7a/liblibprotobuf.a \
    OpenCV/3rdparty.libs/armeabi-v7a/liblibtiff.a \
    OpenCV/3rdparty.libs/armeabi-v7a/liblibwebp.a \
    OpenCV/3rdparty.libs/armeabi-v7a/libquirc.a \
    OpenCV/3rdparty.libs/armeabi-v7a/libtbb.a \
    OpenCV/3rdparty.libs/armeabi-v7a/libtegra_hal.a \
    OpenCV/libs/libopencv_calib3d.a \
    OpenCV/libs/libopencv_core.a \
    OpenCV/libs/libopencv_dnn.a \
    OpenCV/libs/libopencv_features2d.a \
    OpenCV/libs/libopencv_flann.a \
    OpenCV/libs/libopencv_gapi.a \
    OpenCV/libs/libopencv_highgui.a \
    OpenCV/libs/libopencv_imgcodecs.a \
    OpenCV/libs/libopencv_imgproc.a \
    OpenCV/libs/libopencv_java4.so \
    OpenCV/libs/libopencv_ml.a \
    OpenCV/libs/libopencv_objdetect.a \
    OpenCV/libs/libopencv_photo.a \
    OpenCV/libs/libopencv_stitching.a \
    OpenCV/libs/libopencv_video.a \
    OpenCV/libs/libopencv_videoio.a \
    Z390/include/Evolis_Elypso_ErrorCode.xml \
    android/AndroidManifest.xml \
    android/assets/overlayer.bmp \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml \
    android/src/com/evolis/examples/PathUtil.java \
    iso.bmp

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android



RESOURCES += \
    Z390Sample.qrc

ANDROID_EXTRA_LIBS = $$PWD/Z390/lib/armeabi-v7a/libdc_mobile_mate.so\
                     $$PWD/Z390/lib/armeabi-v7a/libdcrf32.so\
                     $$PWD/Z390/lib/armeabi-v7a/libevolis.so\
                     $$PWD/Z390/lib/armeabi-v7a/libEvolis_Z390_LithographPrinter_armeabi-v7a.so\
                     $$PWD/Z390/lib/armeabi-v7a/libwlt2bmp.so\
                     $$PWD/OpenCV/libs/libopencv_java4.so\
                     $$PWD/Fretype2/lib/libfreetyped.so

