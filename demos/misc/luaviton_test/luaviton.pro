
INCLUDEPATH += \
    ../../../code \
    ../../../code/external/lua51

SOURCES += \
    luaviton.cpp

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/lua51/linux-x86_64/ -llua

INCLUDEPATH += $$PWD/../../../code/external/lua5.1
DEPENDPATH += $$PWD/../../../code/external/lua5.1

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/lua51/linux-x86_64/liblua.a

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/ting/linux-x86_64/ -lting

INCLUDEPATH += $$PWD/../../../code/external/ting
DEPENDPATH += $$PWD/../../../code/external/ting

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/ting/linux-x86_64/libting.a

OTHER_FILES += \
    luabridge.lua \
    test.lua

