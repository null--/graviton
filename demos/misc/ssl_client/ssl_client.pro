INCLUDEPATH += \
    ../../../code

SOURCES += \
    ssl_client.cpp

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/openssl/linux-x86_64/ -lssl

INCLUDEPATH += $$PWD/../../../code/external/openssl/include/openssl
DEPENDPATH += $$PWD/../../../code/external/openssl/include/openssl

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/openssl/linux-x86_64/libssl.a

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/ting/linux-x86_64/ -lting

INCLUDEPATH += $$PWD/../../../code/external/ting
DEPENDPATH += $$PWD/../../../code/external/ting

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/ting/linux-x86_64/libting.a

