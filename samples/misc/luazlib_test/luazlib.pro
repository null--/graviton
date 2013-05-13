
INCLUDEPATH += \
    ../../../code \
    ../../../code/external/lua5.1 \
    ../../../code/external/zlib/
	
SOURCES += \
    luazlib.cpp \
    # ../../../code/external/lzlib/lzlib.c

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/lua51/linux-x86_64/ -llua

INCLUDEPATH += $$PWD/../../../code/external/lua5.1
DEPENDPATH += $$PWD/../../../code/external/lua5.1

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/lua51/linux-x86_64/liblua.a

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/zlib/linux-x86_64/ -lzlib

INCLUDEPATH += $$PWD/../../../code/external/zlib
DEPENDPATH += $$PWD/../../../code/external/zlib

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/zlib/linux-x86_64/libzlib.a

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/luazlib/linux-x86_64/ -lluazlib

INCLUDEPATH += $$PWD/../../../code/external/lzlib
DEPENDPATH += $$PWD/../../../code/external/lzlib

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/luazlib/linux-x86_64/libluazlib.a
