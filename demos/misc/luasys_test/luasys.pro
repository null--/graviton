
INCLUDEPATH += \
    ../../../code \
    ../../../code/external/lua5.1 \

SOURCES += \
    luasys.cpp

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/lua51/linux-x86_64/ -llua

INCLUDEPATH += $$PWD/../../../code/external/lua5.1
DEPENDPATH += $$PWD/../../../code/external/lua5.1

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/lua51/linux-x86_64/liblua.a

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/luasys/linux-x86_64/ -lluasys

INCLUDEPATH += $$PWD/../../../code/external/luasys
DEPENDPATH += $$PWD/../../../code/external/luasys

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/luasys/linux-x86_64/libluasys.a
