#include "tst_JsonReader.h"



void tst_JsonReader::initTestCase()
{
    qRegisterMetaType<int>("JsonStreamReader::Error");
}

void tst_JsonReader::init()
{
    mReader = new JsonReader();
}

void tst_JsonReader::cleanup()
{
    if (mReader) {
        delete mReader;
        mReader = 0;
    }
}

void tst_JsonReader::testPrimitiveTypes()
{
    QSignalSpy objectDecodedSpy(mReader, SIGNAL(objectDecoded(const QVariantHash &)));
    QSignalSpy errorSpy(mReader, SIGNAL(error(JsonStreamReader::Error)));
    QList<QVariant> arguments;

    QFile file("data/primitiveTypes.json");
    file.open(QIODevice::ReadOnly);
    mReader->addData(file.readAll());
    mReader->parse();
    
    QVERIFY(objectDecodedSpy.count() == 1);
    QVERIFY(errorSpy.count() == 0);
    arguments = objectDecodedSpy.takeFirst();
    QVariantHash object = arguments.at(0).toHash();
    
    QVERIFY(object.value("string").type() == QVariant::String);
    QCOMPARE(object.value("string").toString(), QString("Hello World!"));
    QVERIFY(object.value("integer").type() == QVariant::LongLong);
    QCOMPARE(object.value("integer").toLongLong(), (qlonglong)42);
    QVERIFY(object.value("double").type() == QVariant::Double);
    QCOMPARE(object.value("double").toDouble(), (double)3.14);
    QVERIFY(object.value("bool").type() == QVariant::Bool);
    QCOMPARE(object.value("bool").toBool(), true);
    QVERIFY(object.value("null").isNull());
}

void tst_JsonReader::testObjectNestedInObject()
{
    QSignalSpy objectDecodedSpy(mReader, SIGNAL(objectDecoded(const QVariantHash &)));
    QSignalSpy errorSpy(mReader, SIGNAL(error(JsonStreamReader::Error)));
    QList<QVariant> arguments;

    QFile file("data/objectNestedInObject.json");
    file.open(QIODevice::ReadOnly);
    mReader->addData(file.readAll());
    mReader->parse();
    
    QVERIFY(objectDecodedSpy.count() == 1);
    QVERIFY(errorSpy.count() == 0);
    arguments = objectDecodedSpy.takeFirst();
    QVariantHash object = arguments.at(0).toHash();
    
    QVERIFY(object.value("params").type() == QVariant::Hash);
    QVariantHash params = object.value("params").toHash();
    QVERIFY(params.value("subtrahend").type() == QVariant::LongLong);
    QCOMPARE(params.value("subtrahend").toLongLong(), (qlonglong)23);
    QVERIFY(params.value("minuend").type() == QVariant::LongLong);
    QCOMPARE(params.value("minuend").toLongLong(), (qlonglong)42);
}

void tst_JsonReader::testArrayNestedInObject()
{
    QSignalSpy objectDecodedSpy(mReader, SIGNAL(objectDecoded(const QVariantHash &)));
    QSignalSpy errorSpy(mReader, SIGNAL(error(JsonStreamReader::Error)));
    QList<QVariant> arguments;

    QFile file("data/arrayNestedInObject.json");
    file.open(QIODevice::ReadOnly);
    mReader->addData(file.readAll());
    mReader->parse();
    
    QVERIFY(objectDecodedSpy.count() == 1);
    QVERIFY(errorSpy.count() == 0);
    arguments = objectDecodedSpy.takeFirst();
    QVariantHash object = arguments.at(0).toHash();
    
    QVERIFY(object.value("params").type() == QVariant::List);
    QVariantList params = object.value("params").toList();
    QVERIFY(params.size() == 2);
    QCOMPARE(params.at(0).toLongLong(), (qlonglong)23);
    QCOMPARE(params.at(1).toLongLong(), (qlonglong)42);
}

void tst_JsonReader::testMultipleObjects()
{
    QSignalSpy objectDecodedSpy(mReader, SIGNAL(objectDecoded(const QVariantHash &)));
    QSignalSpy errorSpy(mReader, SIGNAL(error(JsonStreamReader::Error)));
    QList<QVariant> arguments;

    QFile file("data/multipleObjects.json");
    file.open(QIODevice::ReadOnly);
    mReader->addData(file.readAll());
    mReader->parse();
    
    QVERIFY(objectDecodedSpy.count() == 2);
    QVERIFY(errorSpy.count() == 0);
}

void tst_JsonReader::testIncrementalParsing()
{
    QSignalSpy objectDecodedSpy(mReader, SIGNAL(objectDecoded(const QVariantHash &)));
    QSignalSpy errorSpy(mReader, SIGNAL(error(JsonStreamReader::Error)));

    QString chunk = "{ ";
    mReader->addData(chunk.toUtf8());
    mReader->parse();
    QVERIFY(objectDecodedSpy.count() == 0);
    QVERIFY(errorSpy.count() == 0);
    
    chunk = "\"par";
    mReader->addData(chunk.toUtf8());
    mReader->parse();
    QVERIFY(objectDecodedSpy.count() == 0);
    QVERIFY(errorSpy.count() == 0);
    
    chunk = "ams\": {\"subtrahend\": 2";
    mReader->addData(chunk.toUtf8());
    mReader->parse();
    QVERIFY(objectDecodedSpy.count() == 0);
    QVERIFY(errorSpy.count() == 0);
    
    chunk = "3, \"minuend\":";
    mReader->addData(chunk.toUtf8());
    mReader->parse();
    QVERIFY(objectDecodedSpy.count() == 0);
    QVERIFY(errorSpy.count() == 0);
    
    chunk = " 42} }";
    mReader->addData(chunk.toUtf8());
    mReader->parse();
    QVERIFY(objectDecodedSpy.count() == 1);
    QVERIFY(errorSpy.count() == 0);
    
    
    QList<QVariant> arguments = objectDecodedSpy.takeFirst();
    QVariantHash object = arguments.at(0).toHash();
    
    QVERIFY(object.value("params").type() == QVariant::Hash);
    QVariantHash params = object.value("params").toHash();
    QVERIFY(params.value("subtrahend").type() == QVariant::LongLong);
    QCOMPARE(params.value("subtrahend").toLongLong(), (qlonglong)23);
    QVERIFY(params.value("minuend").type() == QVariant::LongLong);
    QCOMPARE(params.value("minuend").toLongLong(), (qlonglong)42);
}

void tst_JsonReader::testBogusData()
{
    QSignalSpy objectDecodedSpy(mReader, SIGNAL(objectDecoded(const QVariantHash &)));
    QSignalSpy errorSpy(mReader, SIGNAL(error(JsonStreamReader::Error)));

    QString s = "BOGUS";
    mReader->addData(s.toUtf8());
    mReader->parse();
    
    QVERIFY(objectDecodedSpy.count() == 0);
    QVERIFY(errorSpy.count() == 1);
}

QTEST_APPLESS_MAIN(tst_JsonReader)
