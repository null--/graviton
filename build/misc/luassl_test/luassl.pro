INCLUDEPATH += \
    ../../../code

SOURCES += \
    luassl.cpp


unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/luasec/linux-x86_64/ -lluasec

INCLUDEPATH += $$PWD/../../../code/external/luasec
DEPENDPATH += $$PWD/../../../code/external/luasec

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/luasec/linux-x86_64/libluasec.a

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/luasocket/linux-x86_64/ -lluasocket

INCLUDEPATH += $$PWD/../../../code/external/luasocket
DEPENDPATH += $$PWD/../../../code/external/luasocket

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/luasocket/linux-x86_64/libluasocket.a

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/openssl/linux-x86_64/ -lssl

INCLUDEPATH += $$PWD/../../../code/external/openssl/include/openssl
DEPENDPATH += $$PWD/../../../code/external/openssl/include/openssl

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/openssl/linux-x86_64/libssl.a

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/lua51/linux-x86_64/ -llua

INCLUDEPATH += $$PWD/../../../code/external/lua5.1
DEPENDPATH += $$PWD/../../../code/external/lua5.1

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/lua51/linux-x86_64/liblua.a
