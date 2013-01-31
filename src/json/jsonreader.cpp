#include "jsonreader.h"

QTX_BEGIN_NAMESPACE


JsonReader::JsonReader(QObject * parent)
        : QObject(parent),
          mKeepRootContext(false),
          mRootContext(0)
{
}

JsonReader::~JsonReader()
{
    if (mRootContext) {
        delete mRootContext;
    }
}

void JsonReader::addData(const QByteArray & data)
{
    mReader.addData(data);
}

void JsonReader::parse()
{
    while (!mReader.atEnd())
    {
        JsonStreamReader::TokenType token = mReader.readNext();
        switch (token)
        {
            case JsonStreamReader::Invalid:
                processInvalid();
                break;
            case JsonStreamReader::Value:
                processValue();
                break;
            case JsonStreamReader::StartObject:
                processStartObject();
                break;
            case JsonStreamReader::EndObject:
                processEndObject();
                break;
            case JsonStreamReader::StartArray:
                processStartArray();
                break;
            case JsonStreamReader::EndArray:
                processEndArray();
                break;
            default:
                break;
        }
    }
    
    if (JsonStreamReader::NoError != mReader.error()) {
        mErrorString = mReader.errorString();
        emit error(mReader.error());
    }
}

QString JsonReader::errorString() const
{
    return mErrorString;
}

void JsonReader::processStartObject()
{
    //qDebug() << "JsonReader::processStartObject";
    
    QHash<QString, QVariant> newObject;
    JsonReaderContext* context = new JsonReaderContext(mReader.key(), newObject);
    mContexts.push(context);
}

void JsonReader::processEndObject()
{
    //qDebug() << "JsonReader::processEndObject";
    
    if (mContexts.isEmpty()) {
        qWarning() << "WARNING: Invalid JSON parsing context.";
        return;
    }
    
    JsonReaderContext* context = mContexts.pop();
    if (mContexts.isEmpty()) {
        switch(context->type()) {
            case JsonReaderContext::Object: {
                QHash<QString, QVariant> rootObject = context->object();
                emit objectDecoded(rootObject);
                
                if (mKeepRootContext) {
                    mRootContext = context;
                    return;
                }
            }
            break;
            case JsonReaderContext::Array:
            break;
            default:
            break;
        }
    } else {
        JsonReaderContext* parentContext = mContexts.top();
        parentContext->add(context->key(), context->value());
    }
    
    delete context;
}

void JsonReader::processStartArray()
{
    //qDebug() << "JsonReader::processStartArray";
    
    QList<QVariant> newArray;
    JsonReaderContext* context = new JsonReaderContext(mReader.key(), newArray);
    mContexts.push(context);
}

void JsonReader::processEndArray()
{
    //qDebug() << "JsonReader::processEndArray";
    
    if (mContexts.isEmpty()) {
        qWarning() << "WARNING: Invalid JSON parsing context.";
        return;
    }
    
    JsonReaderContext* context = mContexts.pop();
    if (mContexts.isEmpty()) {
        // @todo: Implement support for deserializing arrays as root-level objects.
    } else {
        JsonReaderContext* parentContext = mContexts.top();
        parentContext->add(context->key(), context->value());
    }
    
    delete context;
}

void JsonReader::processValue()
{
    //qDebug() << "JsonReader::processValue";
    
    if (mContexts.isEmpty()) {
        qWarning() << "WARNING: Invalid JSON parsing context.";
        return;
    }
    
    JsonReaderContext* context = mContexts.top();
    context->add(mReader.key(), mReader.value());
}

void JsonReader::processInvalid()
{
    //qDebug() << "JsonReader::processInvalid";
    
    if (mReader.error() == JsonStreamReader::PrematureEndOfDocumentError) {
        // can be recovered by subsequent data parsing
        return;
    }
    
    mErrorString = mReader.errorString();
    emit error(mReader.error());
}


JsonReaderContext::JsonReaderContext(const QString & key, const QHash<QString, QVariant> & object)
    : mType(Object),
      mKey(key),
      mObject(object)
{
}

JsonReaderContext::JsonReaderContext(const QString & key, const QList<QVariant> & array)
    : mType(Array),
      mKey(key),
      mArray(array)
{
}

JsonReaderContext::~JsonReaderContext()
{
}

JsonReaderContext::Type JsonReaderContext::type() const
{
    return mType;
}

QString JsonReaderContext::key() const
{
    return mKey;
}

QHash<QString, QVariant> JsonReaderContext::object() const
{
    return mObject;
}

QList<QVariant> JsonReaderContext::array() const
{
    return mArray;
}

QVariant JsonReaderContext::value() const
{
    switch(mType) {
    case Object:
        return mObject;
    case Array:
        return mArray;
    default:
        break;
    }
    
    return QVariant();
}

bool JsonReaderContext::add(const QString & key, const QVariant & value)
{
    if (mType == Object) {
        mObject.insert(key, value);
        return true;
    } else if (mType == Array) {
        mArray.append(value);
        return true;
    } else {
        qWarning() << "WARNING: Attempt to set value in invalid JSON context.";
    }
    
    return false;
}


QTX_END_NAMESPACE
