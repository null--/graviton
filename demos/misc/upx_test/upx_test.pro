INCLUDEPATH +=  \
        ../../../code
	
SOURCES += \
	upx_test.cpp


# unix:!macx: LIBS += -L$$PWD/../../../libs/ucl/linux-x86_64/ -lucl

# INCLUDEPATH += $$PWD/../../../code/external/ucl
# DEPENDPATH += $$PWD/../../../code/external/ucl

# unix:!macx: PRE_TARGETDEPS += $$PWD/../../../libs/ucl/linux-x86_64/libucl.a

unix:!macx: LIBS += -L$$PWD/../../../libs/upx_ucl/linux-x86_64/ -lupxucl

INCLUDEPATH += $$PWD/../../../code/external/upx
DEPENDPATH += $$PWD/../../../code/external/upx

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../libs/upx_ucl/linux-x86_64/libupxucl.a
