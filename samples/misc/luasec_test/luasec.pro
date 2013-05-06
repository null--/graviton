
INCLUDEPATH += \
    ../../../code \
    ../../../code/external/lua5.1 \
    ../../../code/external/openssl/include/openssl

SOURCES += \
    luasec.cpp \
    # LUASEC SOURCE
    ../../../code/external/luasec/ssl.c \
    ../../../code/external/luasec/buffer.c \
    ../../../code/external/luasec/context.c \
    ../../../code/external/luasec/timeout.c \
    ../../../code/external/luasec/usocket.c \
    ../../../code/external/luasec/io.c \

LIBS += \
    ../../../libs/openssl/linux-x86_64/libssl.a \
    ../../../libs/openssl/linux-x86_64/libcrypto.a \
    ../../../libs/lua51/linux-x86_64/liblua.a \
    ../../../libs/luasec/linux-x86_64/luasec.a
