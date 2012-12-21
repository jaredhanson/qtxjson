#include "jsonstreamreader.h"


static yajl_callbacks callbacks = {  
    json_parse_null,
    json_parse_boolean,
    json_parse_integer,
    json_parse_double,
    //json_parse_number,
    NULL,
    json_parse_string,
    json_parse_start_map,
    json_parse_map_key,
    json_parse_end_map,
    json_parse_start_array,
    json_parse_end_array
};


JsonStreamReader::JsonStreamReader()
    : mError(NoError)
{
    mHandle = yajl_alloc(&callbacks, NULL, (void *)this);
    yajl_config(mHandle, yajl_allow_comments, 1);
    yajl_config(mHandle, yajl_allow_multiple_values, 1);
}

JsonStreamReader::~JsonStreamReader()
{
    if (mHandle) {
        yajl_free(mHandle);
        mHandle = NULL;
    }
}

QString JsonStreamReader::key() const
{
    return mKeyRead;
}

QVariant JsonStreamReader::value() const
{
    return mValueRead;
}

void JsonStreamReader::addData(const QByteArray & data)
{
    yajl_status status = yajl_parse(mHandle, (const unsigned char *)data.data(), data.size());  
    if (status != yajl_status_ok) {
        mError = ParseError;
        unsigned char * str = yajl_get_error(mHandle, 0, NULL, 0);
        mErrorString = QString((const char *)str);
        yajl_free_error(mHandle, str);
        
        this->enqueueToken(JsonStreamReader::Invalid);
    }
}

JsonStreamReader::TokenType JsonStreamReader::readNext()
{
    //qDebug() << "JsonStreamReader::readNext";
    
    if (mTokenQueue.isEmpty()) {
        return NoToken;
    }
    
    TokenType token = mTokenQueue.dequeue();
    mKeyRead = mKeyQueue.dequeue();
    mValueRead = mValueQueue.dequeue();
    
    return token;
}

bool JsonStreamReader::atEnd() const
{
    //qDebug() << "JsonStreamReader::atEnd";
    
    if (NoError != mError) {
        return true;
    }
    
    return mTokenQueue.isEmpty();
}

JsonStreamReader::Error JsonStreamReader::error() const
{
    return mError;
}

QString JsonStreamReader::errorString() const
{
    return mErrorString;
}

void JsonStreamReader::enqueueToken(TokenType type, const QVariant & value /* = QVariant() */)
{
    mTokenQueue.enqueue(type);
    mValueQueue.enqueue(value);
    
    if (mKeyQueue.size() == mValueQueue.size() - 1) {
        mKeyQueue.enqueue(QString());
    } else if (mKeyQueue.size() != mValueQueue.size()) {
        qWarning() << "WARNING: JSON tokenizer is out of sync.";
    }
}

void JsonStreamReader::enqueueKey(const QString & key)
{
    mKeyQueue.enqueue(key);
}



int json_parse_null(void * ctx)
{
    JsonStreamReader* reader = (JsonStreamReader *)ctx;
    reader->enqueueToken(JsonStreamReader::Value, QVariant());
    return 1;
}  
  
int json_parse_boolean(void * ctx, int boolean)
{
    JsonStreamReader* reader = (JsonStreamReader *)ctx;
    reader->enqueueToken(JsonStreamReader::Value, QVariant((bool)boolean));
    return 1;
}

int json_parse_integer(void * ctx, long long integerVal)
{
    JsonStreamReader* reader = (JsonStreamReader *)ctx;
    reader->enqueueToken(JsonStreamReader::Value, QVariant(integerVal));
    return 1;
}

int json_parse_double(void * ctx, double doubleVal)
{
    JsonStreamReader* reader = (JsonStreamReader *)ctx;
    reader->enqueueToken(JsonStreamReader::Value, QVariant(doubleVal));
    return 1;
}

/*
int json_parse_number(void * ctx, const char * numberVal, size_t numberLen)
{
    Q_UNUSED(numberVal);
    Q_UNUSED(numberLen);
    
    JsonStreamReader* reader = (JsonStreamReader *)ctx;
    reader->enqueueToken(JsonStreamReader::Value);
    return 1;
}
*/
  
int json_parse_string(void * ctx, const unsigned char * stringVal, size_t stringLen)
{
    JsonStreamReader* reader = (JsonStreamReader *)ctx;
    reader->enqueueToken(JsonStreamReader::Value, QVariant(QString::fromUtf8((const char *)stringVal, stringLen)));
    return 1;
}

int json_parse_start_map(void * ctx)
{
    JsonStreamReader* reader = (JsonStreamReader *)ctx;
    reader->enqueueToken(JsonStreamReader::StartObject);
    return 1;
}

int json_parse_map_key(void * ctx, const unsigned char * key, size_t stringLen)
{
    JsonStreamReader* reader = (JsonStreamReader *)ctx;
    reader->enqueueKey(QString::fromUtf8((const char *)key, stringLen));
    return 1;
}
  
int json_parse_end_map(void * ctx)
{
    JsonStreamReader* reader = (JsonStreamReader *)ctx;
    reader->enqueueToken(JsonStreamReader::EndObject);
    return 1;
}  

int json_parse_start_array(void * ctx)
{
    JsonStreamReader* reader = (JsonStreamReader *)ctx;
    reader->enqueueToken(JsonStreamReader::StartArray);
    return 1;
}  
  
int json_parse_end_array(void * ctx)
{
    JsonStreamReader* reader = (JsonStreamReader *)ctx;
    reader->enqueueToken(JsonStreamReader::EndArray);
    return 1;
}
