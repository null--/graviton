
INCLUDEPATH += \
    ../../../code \
    ../../../code/external/lua5.1 \
    ../../../code/external/openssl/include/openssl

SOURCES += \
    luasys.cpp

LIBS += \
    ../../../libs/lua51/linux-x86_64/liblua.a \
    ../../../libs/luasys/linux-x86_64/luasys.a
