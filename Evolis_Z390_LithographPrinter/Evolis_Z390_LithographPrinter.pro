QT +=  core gui
android {
QT += androidextras
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
DEFINES += EVOLIS_Z390_LITHOGRAPHPRINTER_LIBRARY\
        TIXML_USE_STL

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS +=  -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-variable

SOURCES += \
    QEvolisPrinter.cpp \
    evolis_z390_lithographprinter.cpp \
    tinyxml/ZBase64.cpp \
    tinyxml/tables.cpp \
    tinyxml/tinystr.cpp \
    tinyxml/tinyxml.cpp \
    tinyxml/tinyxmlerror.cpp \
    tinyxml/tinyxmlparser.cpp
INCLUDEPATH += $$PWD/SDK/Evolis_sdk/Include.2021.05.17\
               $$PWD/SDK/decard

DEPENDPATH+=$$PWD/SDK/Evolis_sdk/lib/libevolis-android-22-armeabi-v7a\
               $$PWD/SDK/decard/armeabi-v7a
HEADERS += \
    Evolis_Z390_LithographPrinter_global.h \
    LithographPrinter.h \
    QEvolisPrinter.h \
    SDK/Evolis_sdk/Include.2021.05.17/android.h \
    SDK/Evolis_sdk/Include.2021.05.17/evo-printers.h \
    SDK/Evolis_sdk/Include.2021.05.17/evolis.h \
    SDK/decard/BasicOper.h \
    SDK/decard/dcrf32.h \
    evolis_z390_lithographprinter.h \
    tinyxml/ZBase64.h \
    tinyxml/tables.h \
    tinyxml/targetver.h \
    tinyxml/tinystr.h \
    tinyxml/tinyxml.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target



ANDROID_EXTRA_LIBS = $$PWD/SDK/decard/armeabi-v7a/libdc_mobile_mate.so\
                     $$PWD/SDK/decard/armeabi-v7a/libdcrf32.so\
                     $$PWD/SDK/decard/armeabi-v7a/libwlt2bmp.so
                     $$PWD/SDK/Evolis_sdk/lib/libevolis-android-22-armeabi-v7a/libevolis.so


unix:!macx: LIBS += -L$$PWD/SDK/decard/armeabi-v7a/ -ldc_mobile_mate

INCLUDEPATH += $$PWD/SDK/decard/armeabi-v7a
DEPENDPATH += $$PWD/SDK/decard/armeabi-v7a

unix:!macx: LIBS += -L$$PWD/SDK/decard/armeabi-v7a/ -ldcrf32

INCLUDEPATH += $$PWD/SDK/decard/armeabi-v7a
DEPENDPATH += $$PWD/SDK/decard/armeabi-v7a

unix:!macx: LIBS += -L$$PWD/SDK/decard/armeabi-v7a/ -lwlt2bmp

INCLUDEPATH += $$PWD/SDK/decard/armeabi-v7a
DEPENDPATH += $$PWD/SDK/decard/armeabi-v7a

unix:!macx: LIBS += -L$$PWD/SDK/Evolis_sdk/lib/libevolis-android-22-armeabi-v7a/ -levolis

INCLUDEPATH += $$PWD/SDK/Evolis_sdk/lib/libevolis-android-22-armeabi-v7a
DEPENDPATH += $$PWD/SDK/Evolis_sdk/lib/libevolis-android-22-armeabi-v7a


QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-variable

#win32{
#    CONFIG(release,debug|release){
#        target.path = E:/SVN/Evolis_Z390_LithographPrinter/Z390Sample/Z390/lib/armeabi-v7a
#        target.files += E:/SVN/Evolis_Z390_LithographPrinter/build-Evolis_Z390_LithographPrinter-Android_Qt_5_15_2_Clang_Multi_Abi-Debug/libEvolis_Z390_LithographPrinter_armeabi-v7a.so
#    }
#}

RESOURCES += \
    Z390.qrc

