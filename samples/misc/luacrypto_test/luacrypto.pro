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

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/openssl/linux-x86_64/libcrypto.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../libs/luacrypto/linux-x86_64/release/ -lluacrypto
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../libs/luacrypto/linux-x86_64/debug/ -lluacrypto
else:symbian: LIBS += -lluacrypto
else:unix: LIBS += -L$$PWD/../../../libs/luacrypto/linux-x86_64/ -lluacrypto

INCLUDEPATH += $$PWD/../../../code/external/luacrypto
DEPENDPATH += $$PWD/../../../code/external/luacrypto

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../libs/luacrypto/linux-x86_64/release/luacrypto.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../libs/luacrypto/linux-x86_64/debug/luacrypto.lib
else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/luacrypto/linux-x86_64/libluacrypto.a
