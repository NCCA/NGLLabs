QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
OBJECTS_DIR = $$PWD/obj
MOC_DIR = $$PWD/moc
SOURCES += \
    $$PWD/src/main.cpp \
    $$PWD/src/MainWindow.cpp \
    $$PWD/src/Emitter.cpp \
    $$PWD/src/NGLScene.cpp \
    $$PWD/src/NGLSceneMouseControls.cpp

HEADERS += \
    $$PWD/include/MainWindow.h \
    $$PWD/include/Emitter.h \
    $$PWD/include/NGLScene.h \
    $$PWD/include/Particle.h \
    $$PWD/include/WindowParams.h 

FORMS += \
    $$PWD/ui/MainWindow.ui
INCLUDEPATH += include
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


NGLPATH=$$(NGLDIR)
isEmpty(NGLPATH){ # note brace must be here
	message("including $HOME/NGL")
	include($(HOME)/NGL/UseNGL.pri)
	win32:include($(HOMEDRIVE)\$(HOMEPATH)\NGL\UseNGL.pri)

}
else{ # note brace must be here
	message("Using custom NGL location")
	include($(NGLDIR)/UseNGL.pri)
}



