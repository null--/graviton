SOURCES += \
    luacrypto.cpp

INCLUDEPATH += \
    ../../../code/

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/lua51/linux-x86_64/ -llua

INCLUDEPATH += $$PWD/../../../code/external/lua5.1
DEPENDPATH += $$PWD/../../../code/external/lua5.1

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/lua51/linux-x86_64/liblua.a

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/openssl/linux-x86_64/ -lcrypto

INCLUDEPATH += $$PWD/../../../code/external/openssl/include/openssl
DEPENDPATH += $$PWD/../../../code/external/openssl/include/openssl


unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/luacrypto/linux-x86_64/ -lluacrypto

INCLUDEPATH += $$PWD/../../../code/external/luacrypto
DEPENDPATH += $$PWD/../../../code/external/luacrypto

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/luacrypto/linux-x86_64/libluacrypto.a
