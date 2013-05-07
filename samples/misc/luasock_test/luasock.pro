
INCLUDEPATH += \
    ../../../code
	
SOURCES += \
    luasock.cpp \
#    ../../../code/external/luasocket/usocket.c \
#    ../../../code/external/luasocket/unix.c \
#    ../../../code/external/luasocket/udp.c \
#    ../../../code/external/luasocket/timeout.c \
#    ../../../code/external/luasocket/tcp.c \
#    ../../../code/external/luasocket/select.c \
#    ../../../code/external/luasocket/options.c \
#    ../../../code/external/luasocket/mime.c \
#    ../../../code/external/luasocket/luasocket.c \
#    ../../../code/external/luasocket/io.c \
#    ../../../code/external/luasocket/inet.c \
#    ../../../code/external/luasocket/except.c \
#    ../../../code/external/luasocket/buffer.c \
#    ../../../code/external/luasocket/auxiliar.c

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/luasocket/linux-x86_64/ -lluasocket

INCLUDEPATH += $$PWD/../../../code/external/luasocket
DEPENDPATH += $$PWD/../../../code/external/luasocket

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/luasocket/linux-x86_64/libluasocket.a

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/lua51/linux-x86_64/ -llua

INCLUDEPATH += $$PWD/../../../code/external/lua5.1
DEPENDPATH += $$PWD/../../../code/external/lua5.1

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/lua51/linux-x86_64/liblua.a
