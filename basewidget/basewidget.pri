INCLUDEPATH += $$PWD
#如果DEPENDPATH目录里的头文件发生了变化，所以cpp都会重新编译
DEPENDPATH += $$PWD

HEADERS += \
        $$PWD/animationstackedwidget.h \
        $$PWD/iplineedit.h \
        $$PWD/customwidget.h \
        $$PWD/clineedit.h

SOURCES += \
        $$PWD/animationstackedwidget.cpp \
        $$PWD/iplineedit.cpp \
        $$PWD/customwidget.cpp \
        $$PWD/clineedit.cpp
