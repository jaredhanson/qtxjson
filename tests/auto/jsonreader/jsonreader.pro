TEMPLATE = app
TARGET = tst_jsonreader

QT += testlib
macx:CONFIG -= app_bundle

include(../../../mkspecs/test.pri)

HEADERS += tst_jsonreader.h
SOURCES += tst_jsonreader.cpp
