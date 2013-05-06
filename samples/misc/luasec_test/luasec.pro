INCLUDEPATH += \
    ../../../code \
    ../../../code/external/lua5.1 \
    ../../../code/external/openssl/include/openssl

SOURCES += \
    luasec.cpp \
    ../../../code/external/luasec/context.c \
    ../../../code/external/luasec/ssl.c

LIBS += \
    ../../../libs/openssl/linux-x86_64/libssl.a \
    ../../../libs/openssl/linux-x86_64/libcrypto.a \
    ../../../libs/lua51/linux-x86_64/liblua.a \
    ../../../libs/luasec/linux-x86_64/luasec.a
