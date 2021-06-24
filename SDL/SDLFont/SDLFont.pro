QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH += $$PWD/SDL2/include/SDL2
DEPENDPATH += $$PWD/SDL2/include/SDL2


INCLUDEPATH += $$PWD/Freetype/include/freetype2
DEPENDPATH += $$PWD/Freetype/include/freetype2

INCLUDEPATH += $$PWD/Freetype/include/freetype2/freetype
DEPENDPATH += $$PWD/Freetype/include/freetype2/freetype


unix:!macx: LIBS += -L$$PWD/SDL_ttf/lib/ -lSDL2_ttf

INCLUDEPATH += $$PWD/SDL_ttf
DEPENDPATH += $$PWD/SDL_ttf

unix:!macx: PRE_TARGETDEPS += $$PWD/SDL_ttf/lib/libSDL2_ttf.a

unix:!macx: LIBS += -L$$PWD/Freetype/lib/ -lfreetyped

INCLUDEPATH += $$PWD/Freetype/include
DEPENDPATH += $$PWD/Freetype/include

unix:!macx: PRE_TARGETDEPS += $$PWD/Freetype/lib/libfreetyped.a

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml \
    android/src/org/libsdl/app/HIDDevice.java \
    android/src/org/libsdl/app/HIDDeviceBLESteamController.java \
    android/src/org/libsdl/app/HIDDeviceManager.java \
    android/src/org/libsdl/app/HIDDeviceUSB.java \
    android/src/org/libsdl/app/SDL.java \
    android/src/org/libsdl/app/SDLActivity.java \
    android/src/org/libsdl/app/SDLAudioManager.java \
    android/src/org/libsdl/app/SDLControllerManager.java

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

RESOURCES += \
    SDLFont.qrc



unix:!macx: LIBS += -L$$PWD/SDL2/lib/ -lSDL2

INCLUDEPATH += $$PWD/SDL2/include
DEPENDPATH += $$PWD/SDL2/include

ANDROID_EXTRA_LIBS = $$PWD/SDL2/lib/libhidapi.so $$PWD/SDL2/lib/libSDL2.so
