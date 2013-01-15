#ifndef TST_JSONREADER_H
#define TST_JSONREADER_H

#include "jsonreader.h"
#include <QtTest/QtTest>

QTX_USE_NAMESPACE


class tst_JsonReader : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void cleanup();
    
    void testPrimitiveTypes();
    void testObjectNestedInObject();
    void testArrayNestedInObject();
    void testMultipleObjects();
    
    void testIncrementalParsing();
    void testBogusData();
    
private:
    JsonReader* mReader;
};

#endif // TST_JSONREADER_H
