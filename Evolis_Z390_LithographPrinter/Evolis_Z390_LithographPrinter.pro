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
    CVText_CN.h \
    Evolis_Z390_LithographPrinter_global.h \
    Fretype2/Include/freetype/config/ftconfig.h \
    Fretype2/Include/freetype/config/ftheader.h \
    Fretype2/Include/freetype/config/ftmodule.h \
    Fretype2/Include/freetype/config/ftoption.h \
    Fretype2/Include/freetype/config/ftstdlib.h \
    Fretype2/Include/freetype/config/integer-types.h \
    Fretype2/Include/freetype/config/mac-support.h \
    Fretype2/Include/freetype/config/public-macros.h \
    Fretype2/Include/freetype/freetype.h \
    Fretype2/Include/freetype/ftadvanc.h \
    Fretype2/Include/freetype/ftbbox.h \
    Fretype2/Include/freetype/ftbdf.h \
    Fretype2/Include/freetype/ftbitmap.h \
    Fretype2/Include/freetype/ftbzip2.h \
    Fretype2/Include/freetype/ftcache.h \
    Fretype2/Include/freetype/ftchapters.h \
    Fretype2/Include/freetype/ftcid.h \
    Fretype2/Include/freetype/ftcolor.h \
    Fretype2/Include/freetype/ftdriver.h \
    Fretype2/Include/freetype/fterrdef.h \
    Fretype2/Include/freetype/fterrors.h \
    Fretype2/Include/freetype/ftfntfmt.h \
    Fretype2/Include/freetype/ftgasp.h \
    Fretype2/Include/freetype/ftglyph.h \
    Fretype2/Include/freetype/ftgxval.h \
    Fretype2/Include/freetype/ftgzip.h \
    Fretype2/Include/freetype/ftimage.h \
    Fretype2/Include/freetype/ftincrem.h \
    Fretype2/Include/freetype/ftlcdfil.h \
    Fretype2/Include/freetype/ftlist.h \
    Fretype2/Include/freetype/ftlzw.h \
    Fretype2/Include/freetype/ftmac.h \
    Fretype2/Include/freetype/ftmm.h \
    Fretype2/Include/freetype/ftmodapi.h \
    Fretype2/Include/freetype/ftmoderr.h \
    Fretype2/Include/freetype/ftotval.h \
    Fretype2/Include/freetype/ftoutln.h \
    Fretype2/Include/freetype/ftparams.h \
    Fretype2/Include/freetype/ftpfr.h \
    Fretype2/Include/freetype/ftrender.h \
    Fretype2/Include/freetype/ftsizes.h \
    Fretype2/Include/freetype/ftsnames.h \
    Fretype2/Include/freetype/ftstroke.h \
    Fretype2/Include/freetype/ftsynth.h \
    Fretype2/Include/freetype/ftsystem.h \
    Fretype2/Include/freetype/fttrigon.h \
    Fretype2/Include/freetype/fttypes.h \
    Fretype2/Include/freetype/ftwinfnt.h \
    Fretype2/Include/freetype/internal/autohint.h \
    Fretype2/Include/freetype/internal/cffotypes.h \
    Fretype2/Include/freetype/internal/cfftypes.h \
    Fretype2/Include/freetype/internal/compiler-macros.h \
    Fretype2/Include/freetype/internal/ftcalc.h \
    Fretype2/Include/freetype/internal/ftdebug.h \
    Fretype2/Include/freetype/internal/ftdrv.h \
    Fretype2/Include/freetype/internal/ftgloadr.h \
    Fretype2/Include/freetype/internal/fthash.h \
    Fretype2/Include/freetype/internal/ftmemory.h \
    Fretype2/Include/freetype/internal/ftobjs.h \
    Fretype2/Include/freetype/internal/ftpsprop.h \
    Fretype2/Include/freetype/internal/ftrfork.h \
    Fretype2/Include/freetype/internal/ftserv.h \
    Fretype2/Include/freetype/internal/ftstream.h \
    Fretype2/Include/freetype/internal/fttrace.h \
    Fretype2/Include/freetype/internal/ftvalid.h \
    Fretype2/Include/freetype/internal/psaux.h \
    Fretype2/Include/freetype/internal/pshints.h \
    Fretype2/Include/freetype/internal/services/svbdf.h \
    Fretype2/Include/freetype/internal/services/svcfftl.h \
    Fretype2/Include/freetype/internal/services/svcid.h \
    Fretype2/Include/freetype/internal/services/svfntfmt.h \
    Fretype2/Include/freetype/internal/services/svgldict.h \
    Fretype2/Include/freetype/internal/services/svgxval.h \
    Fretype2/Include/freetype/internal/services/svkern.h \
    Fretype2/Include/freetype/internal/services/svmetric.h \
    Fretype2/Include/freetype/internal/services/svmm.h \
    Fretype2/Include/freetype/internal/services/svotval.h \
    Fretype2/Include/freetype/internal/services/svpfr.h \
    Fretype2/Include/freetype/internal/services/svpostnm.h \
    Fretype2/Include/freetype/internal/services/svprop.h \
    Fretype2/Include/freetype/internal/services/svpscmap.h \
    Fretype2/Include/freetype/internal/services/svpsinfo.h \
    Fretype2/Include/freetype/internal/services/svsfnt.h \
    Fretype2/Include/freetype/internal/services/svttcmap.h \
    Fretype2/Include/freetype/internal/services/svtteng.h \
    Fretype2/Include/freetype/internal/services/svttglyf.h \
    Fretype2/Include/freetype/internal/services/svwinfnt.h \
    Fretype2/Include/freetype/internal/sfnt.h \
    Fretype2/Include/freetype/internal/t1types.h \
    Fretype2/Include/freetype/internal/tttypes.h \
    Fretype2/Include/freetype/internal/wofftypes.h \
    Fretype2/Include/freetype/t1tables.h \
    Fretype2/Include/freetype/ttnameid.h \
    Fretype2/Include/freetype/tttables.h \
    Fretype2/Include/freetype/tttags.h \
    Fretype2/Include/ft2build.h \
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


INCLUDEPATH += $$PWD/Fretype2/include
DEPENDPATH += $$PWD/Fretype2/include

INCLUDEPATH += $$PWD/OpenCV
INCLUDEPATH += $$PWD/OpenCV/3rdparty.libs/armeabi-v7a
DEPENDPATH += $$PWD/OpenCV/3rdparty.libs/armeabi-v7a

INCLUDEPATH += $$PWD/OpenCV/libs
DEPENDPATH += $$PWD/OpenCV/libs

unix:!macx: LIBS += -L$$PWD/OpenCV/libs/ -lopencv_calib3d
unix:!macx: LIBS += -L$$PWD/OpenCV/libs/ -lopencv_core
unix:!macx: LIBS += -L$$PWD/OpenCV/libs/ -lopencv_dnn
unix:!macx: LIBS += -L$$PWD/OpenCV/libs/ -lopencv_features2d
unix:!macx: LIBS += -L$$PWD/OpenCV/libs/ -lopencv_flann
unix:!macx: LIBS += -L$$PWD/OpenCV/libs/ -lopencv_gapi
unix:!macx: LIBS += -L$$PWD/OpenCV/libs/ -lopencv_highgui
unix:!macx: LIBS += -L$$PWD/OpenCV/libs/ -lopencv_imgcodecs
unix:!macx: LIBS += -L$$PWD/OpenCV/libs/ -lopencv_imgproc
unix:!macx: LIBS += -L$$PWD/OpenCV/libs/ -lopencv_ml
unix:!macx: LIBS += -L$$PWD/OpenCV/libs/ -lopencv_objdetect
unix:!macx: LIBS += -L$$PWD/OpenCV/libs/ -lopencv_photo
unix:!macx: LIBS += -L$$PWD/OpenCV/libs/ -lopencv_stitching
unix:!macx: LIBS += -L$$PWD/OpenCV/libs/ -lopencv_video
unix:!macx: LIBS += -L$$PWD/OpenCV/libs/ -lopencv_videoio

unix:!macx: LIBS += -L$$PWD/OpenCV/3rdparty.libs/armeabi-v7a/ -lade
unix:!macx: LIBS += -L$$PWD/OpenCV/3rdparty.libs/armeabi-v7a/ -lcpufeatures
unix:!macx: LIBS += -L$$PWD/OpenCV/3rdparty.libs/armeabi-v7a/ -lIlmImf
unix:!macx: LIBS += -L$$PWD/OpenCV/3rdparty.libs/armeabi-v7a/ -littnotify
unix:!macx: LIBS += -L$$PWD/OpenCV/3rdparty.libs/armeabi-v7a/ -llibjpeg-turbo
unix:!macx: LIBS += -L$$PWD/OpenCV/3rdparty.libs/armeabi-v7a/ -llibopenjp2
unix:!macx: LIBS += -L$$PWD/OpenCV/3rdparty.libs/armeabi-v7a/ -llibpng
unix:!macx: LIBS += -L$$PWD/OpenCV/3rdparty.libs/armeabi-v7a/ -llibprotobuf
unix:!macx: LIBS += -L$$PWD/OpenCV/3rdparty.libs/armeabi-v7a/ -llibtiff
unix:!macx: LIBS += -L$$PWD/OpenCV/3rdparty.libs/armeabi-v7a/ -llibwebp
unix:!macx: LIBS += -L$$PWD/OpenCV/3rdparty.libs/armeabi-v7a/ -lquirc
unix:!macx: LIBS += -L$$PWD/OpenCV/3rdparty.libs/armeabi-v7a/ -ltbb
unix:!macx: LIBS += -L$$PWD/OpenCV/3rdparty.libs/armeabi-v7a/ -ltegra_hal

unix:!macx: LIBS += -L$$PWD/OpenCV/libs/ -lopencv_java4

ANDROID_EXTRA_LIBS = $$PWD/OpenCV/libs/libopencv_java4.so

unix:!macx: LIBS += -L$$PWD/Fretype2/lib/ -lfreetyped

INCLUDEPATH += $$PWD/Fretype2/include
DEPENDPATH += $$PWD/Fretype2/include

unix:!macx: PRE_TARGETDEPS += $$PWD/Fretype2/lib/libfreetyped.a
