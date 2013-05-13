SOURCES += \
    luadigest.cpp

INCLUDEPATH += \
    ../../../code/

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/lua51/linux-x86_64/ -llua

INCLUDEPATH += $$PWD/../../../code/external/lua5.1
DEPENDPATH += $$PWD/../../../code/external/lua5.1

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/lua51/linux-x86_64/liblua.a

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/openssl/linux-x86_64/ -lcrypto

INCLUDEPATH += $$PWD/../../../code/external/openssl/include/openssl
DEPENDPATH += $$PWD/../../../code/external/openssl/include/openssl


unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/ldigest/linux-x86_64/ -lldigest

INCLUDEPATH += $$PWD/../../../code/external/ldigest
DEPENDPATH += $$PWD/../../../code/external/ldigest

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/ldigest/linux-x86_64/libldigest.a

OTHER_FILES += \
    test.lua
