QT +=  core
#android {
#QT += androidextras
#}


TEMPLATE = lib
DEFINES += EVOLIS_Z390_LITHOGRAPHPRINTER_LIBRARY\
        TIXML_USE_STL

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



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





RESOURCES += \
    Z390.qrc


INCLUDEPATH += $$PWD/opencv2
DEPENDPATH += $$PWD/opencv2

unix:!macx: LIBS += -L$$PWD/opencv2/lib/ -lopencv_world
unix:!macx: PRE_TARGETDEPS += $$PWD/opencv2/lib/libopencv_world.a

unix:!macx: LIBS += -L$$PWD/opencv2/lib/ -lcpufeatures
unix:!macx: PRE_TARGETDEPS += $$PWD/opencv2/lib/libcpufeatures.a

unix:!macx: LIBS += -L$$PWD/opencv2/lib/ -llibpng
unix:!macx: PRE_TARGETDEPS += $$PWD/opencv2/lib/liblibpng.a

unix:!macx: LIBS += -L$$PWD/opencv2/lib/ -ltegra_hal
unix:!macx: PRE_TARGETDEPS += $$PWD/opencv2/lib/libtegra_hal.a

unix:!macx: LIBS += -L$$PWD/opencv2/lib/ -lcpufeatures
unix:!macx: PRE_TARGETDEPS += $$PWD/opencv2/lib/libcpufeatures.a

unix:!macx: LIBS += -L$$PWD/opencv2/lib/ -llibopenjp2
unix:!macx: PRE_TARGETDEPS += $$PWD/opencv2/lib/liblibopenjp2.a

unix:!macx: LIBS += -L$$PWD/opencv2/lib/ -llibjpeg-turbo
unix:!macx: PRE_TARGETDEPS += $$PWD/opencv2/lib/liblibjpeg-turbo.a

unix:!macx: LIBS += -L$$PWD/harfbuzz/lib/ -lharfbuzz
INCLUDEPATH += $$PWD/harfbuzz/include
DEPENDPATH += $$PWD/harfbuzz/include
unix:!macx: PRE_TARGETDEPS += $$PWD/harfbuzz/lib/libharfbuzz.a

unix:!macx: LIBS += -L$$PWD/freetype/lib/ -lfreetype
INCLUDEPATH += $$PWD/freetype/include
DEPENDPATH += $$PWD/freetype/include
unix:!macx: PRE_TARGETDEPS += $$PWD/freetype/lib/libfreetype.a

QMAKE_POST_LINK+= $$PWD/install.bat


QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-variable
