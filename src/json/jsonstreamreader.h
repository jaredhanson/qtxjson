#ifndef JSONSTREAMREADER_H
#define JSONSTREAMREADER_H

#include <QtCore>
#include <yajl/yajl_parse.h>


extern int json_parse_null(void * ctx);
extern int json_parse_boolean(void * ctx, int boolean);
extern int json_parse_integer(void * ctx, long long integerVal);
extern int json_parse_double(void * ctx, double doubleVal);
//extern int json_parse_number(void * ctx, const char * numberVal, size_t numberLen);
extern int json_parse_string(void * ctx, const unsigned char * stringVal, size_t stringLen);
extern int json_parse_start_map(void * ctx);
extern int json_parse_map_key(void * ctx, const unsigned char * key, size_t stringLen);
extern int json_parse_end_map(void * ctx);
extern int json_parse_start_array(void * ctx);
extern int json_parse_end_array(void * ctx);

class JsonStreamReader
{
public:
    typedef enum {
        NoToken,
        Invalid,
        Value,
        StartObject,
        EndObject,
        StartArray,
        EndArray,
    } TokenType;
    
    typedef enum {
        NoError,
        ParseError,
        PrematureEndOfDocumentError
    } Error;
    
public:
    JsonStreamReader();
    ~JsonStreamReader();

    QString key() const;
    QVariant value() const;

    void addData(const QByteArray & data);
    TokenType readNext();
    bool atEnd() const;
    
    Error error() const;
    QString errorString() const;

private:
    void enqueueToken(TokenType token, const QVariant & value = QVariant());
    void enqueueKey(const QString & key);
    
    yajl_handle mHandle;
    QQueue<TokenType> mTokenQueue;
    QQueue<QString> mKeyQueue;
    QQueue<QVariant> mValueQueue;
    
    QString mKeyRead;
    QVariant mValueRead;
    
    Error mError;
    QString mErrorString;
    
private:
    friend int json_parse_null(void * ctx);
    friend int json_parse_boolean(void * ctx, int boolean);
    friend int json_parse_integer(void * ctx, long long integerVal);
    friend int json_parse_double(void * ctx, double doubleVal);
    //friend int json_parse_number(void * ctx, const char * numberVal, size_t numberLen);
    friend int json_parse_string(void * ctx, const unsigned char * stringVal, size_t stringLen);
    friend int json_parse_start_map(void * ctx);
    friend int json_parse_map_key(void * ctx, const unsigned char * key, size_t stringLen);
    friend int json_parse_end_map(void * ctx);
    friend int json_parse_start_array(void * ctx);
    friend int json_parse_end_array(void * ctx);
};

#endif // JSONSTREAMREADER_H
