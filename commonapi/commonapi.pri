INCLUDEPATH += $$PWD
#如果DEPENDPATH目录里的头文件发生了变化，所以cpp都会重新编译
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/global.h \
    $$PWD/myapp.h \
    $$PWD/qqcell.h \
    $$PWD/iteminfo.h \
    $$PWD/unit.h


SOURCES += \
    $$PWD/myapp.cpp \
    $$PWD/iteminfo.cpp \
    $$PWD/qqcell.cpp

