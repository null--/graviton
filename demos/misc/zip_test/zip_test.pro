
INCLUDEPATH += \
    ../../../code \
	
SOURCES += \
    zip_test.cpp \
    # ../../../code/external/lzlib/lzlib.c

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/lua51/linux-x86_64/liblua.a

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/zlib/linux-x86_64/ -lzlib

INCLUDEPATH += $$PWD/../../../code/external/zlib
DEPENDPATH += $$PWD/../../../code/external/zlib

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/luazlib/linux-x86_64/libluazlib.a
