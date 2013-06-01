SOURCES += \
    crypto.cpp

INCLUDEPATH += \
    ../../../code/

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/openssl/linux-x86_64/ -lcrypto

INCLUDEPATH += $$PWD/../../../code/external/openssl/include/openssl
DEPENDPATH += $$PWD/../../../code/external/openssl/include/openssl

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/ting/linux-x86_64/ -lting

INCLUDEPATH += $$PWD/../../../code/external/ting
DEPENDPATH += $$PWD/../../../code/external/ting
