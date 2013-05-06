
INCLUDEPATH += \
    ../../../code \
    ../../../code/external/lua5.1
	
SOURCES += \
    luasock.cpp

LIBS += \
    ../../../libs/lua51/linux-x86_64/liblua.a \
    ../../../libs/luasocket/linux-x86_64/luasocket.a
