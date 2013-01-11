#include "jsonwriter.h"

QTX_BEGIN_NAMESPACE


QString JsonWriter::stringify(const QHash<QString, QVariant> & object)
{
    JsonWriter writer;
    QByteArray bytes = writer.write(object);
    return QString::fromUtf8(bytes);
}

JsonWriter::JsonWriter()
{
}

JsonWriter::~JsonWriter()
{
}

QByteArray JsonWriter::write(const QHash<QString, QVariant> & object)
{
    writeObject(object);   
    return mWriter.data();
}

void JsonWriter::writeValue(const QVariant & value)
{
    switch(value.type())
    {
    case QVariant::Invalid:
        mWriter.writeNull();
        break;
    case QVariant::Bool:
        mWriter.writeBool(value.toBool());
        break;
    case QVariant::Int:
        mWriter.writeInteger(value.toInt());
        break;
    case QVariant::UInt:
        mWriter.writeInteger(value.toUInt());
        break;
    case QVariant::LongLong:
        mWriter.writeInteger(value.toLongLong());
        break;
    case QVariant::ULongLong:
        mWriter.writeInteger(value.toULongLong());
        break;
    case QVariant::Double:
        mWriter.writeDouble(value.toDouble());
        break;
    case QVariant::String:
        mWriter.writeString(value.toString());
        break;
    case QVariant::Hash:
        writeObject(value.toHash());
        break;
    case QVariant::List:
        writeArray(value.toList());
        break;
    default:
        break;
    }
    
}

void JsonWriter::writeObject(const QHash<QString, QVariant> & object)
{
    mWriter.writeStartObject();
    
    QHashIterator<QString, QVariant> itr(object);
    while (itr.hasNext()) {
        itr.next();
        mWriter.writeKey(itr.key());
        writeValue(itr.value());
    }
    
    mWriter.writeEndObject();
}

void JsonWriter::writeArray(const QList<QVariant> & array)
{
    mWriter.writeStartArray();
    
    QListIterator<QVariant> itr(array);
    while (itr.hasNext()) {
        writeValue(itr.next());
    }
    
    mWriter.writeEndArray();
}


QTX_END_NAMESPACE
