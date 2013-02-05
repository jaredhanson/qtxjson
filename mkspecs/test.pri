include(features/qtx_testcase.prf)


INCLUDEPATH += ../../../src/json \
               ../../../vendor/include

LIBS += -L../../../lib

LIBS += -lQtxJson
