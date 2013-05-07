
INCLUDEPATH += \
    ../../../code \
    ../../../code/external/lua5.1 \
    ../../../code/external/zlib/
	
SOURCES += \
    luazlib.cpp \
    # ../../../code/external/lzlib/lzlib.c

LIBS += \
    ../../../libs/lua51/linux-x86_64/liblua.a \
    ../../../libs/zlib/linux-x86_64/zlib.a \
    ../../../libs/luazlib/linux-x86_64/luazlib.a
    # -L"../../../libs/luazlib/linux-x86_64/" -l"luazlib"
