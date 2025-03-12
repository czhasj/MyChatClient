INCLUDEPATH += $$PWD
#如果DEPENDPATH目录里的头文件发生了变化，所以cpp都会重新编译
DEPENDPATH += $$PWD

HEADERS += \
        $$PWD/animationstackedwidget.h \
        $$PWD/chatstackedwidget.h \
        $$PWD/iplineedit.h \
        $$PWD/customwidget.h \
        $$PWD/clineedit.h \
        $$PWD/qqlist/qqlistviewchild.h \
        $$PWD/qqlist/qqlistviewgroup.h \
        $$PWD/qqlist/qqlistwidget.h \
        $$PWD/qqlist/qqpopmenuwidget.h

SOURCES += \
        $$PWD/animationstackedwidget.cpp \
        $$PWD/chatstackedwidget.cpp \
        $$PWD/iplineedit.cpp \
        $$PWD/customwidget.cpp \
        $$PWD/clineedit.cpp \
        $$PWD/qqlist/qqlistviewchild.cpp \
        $$PWD/qqlist/qqlistviewgroup.cpp \
        $$PWD/qqlist/qqlistwidget.cpp \
        $$PWD/qqlist/qqpopmenuwidget.cpp

FORMS += \
    $$PWD/qqlist/qqlistviewgroup.ui
