SOURCES += \
    luaall.cpp

INCLUDEPATH += \
    ../../../code/

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/lua51/linux-x86_64/ -llua

INCLUDEPATH += $$PWD/../../../code/external/lua5.1
DEPENDPATH += $$PWD/../../../code/external/lua5.1

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/lua51/linux-x86_64/liblua.a

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/openssl/linux-x86_64/ -lcrypto

INCLUDEPATH += $$PWD/../../../code/external/openssl/include/openssl
DEPENDPATH += $$PWD/../../../code/external/openssl/include/openssl

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/openssl/linux-x86_64/ -lssl

INCLUDEPATH += $$PWD/../../../code/external/openssl/include/openssl
DEPENDPATH += $$PWD/../../../code/external/openssl/include/openssl

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/openssl/linux-x86_64/libssl.a

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/luacrypto/linux-x86_64/ -lluacrypto

INCLUDEPATH += $$PWD/../../../code/external/luacrypto
DEPENDPATH += $$PWD/../../../code/external/luacrypto

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/luacrypto/linux-x86_64/libluacrypto.a

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/luasocket/linux-x86_64/ -lluasocket

INCLUDEPATH += $$PWD/../../../code/external/luasocket
DEPENDPATH += $$PWD/../../../code/external/luasocket

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/luasocket/linux-x86_64/libluasocket.a

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/luasec/linux-x86_64/ -lluasec

INCLUDEPATH += $$PWD/../../../code/external/luasec
DEPENDPATH += $$PWD/../../../code/external/luasec

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/luasec/linux-x86_64/libluasec.a

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/luasys/linux-x86_64/ -lluasys

INCLUDEPATH += $$PWD/../../../code/external/luasys
DEPENDPATH += $$PWD/../../../code/external/luasys

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/luasys/linux-x86_64/libluasys.a

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/zlib/linux-x86_64/ -lzlib

INCLUDEPATH += $$PWD/../../../code/external/zlib
DEPENDPATH += $$PWD/../../../code/external/zlib

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/zlib/linux-x86_64/libzlib.a

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/luazlib/linux-x86_64/ -lluazlib

INCLUDEPATH += $$PWD/../../../code/external/lzlib
DEPENDPATH += $$PWD/../../../code/external/lzlib

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/luazlib/linux-x86_64/libluazlib.a
