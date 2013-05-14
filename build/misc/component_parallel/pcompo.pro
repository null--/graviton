
SOURCES += \
        pcompo.cpp

INCLUDEPATH += \
    ../../../code

unix:!macx:!symbian: LIBS += -L$$PWD/../../../libs/ting/linux-x86_64/ -lting

INCLUDEPATH += $$PWD/../../../code/external/ting
DEPENDPATH += $$PWD/../../../code/external/ting

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../libs/ting/linux-x86_64/libting.a
