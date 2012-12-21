#include "jsonstreamwriter.h"


JsonStreamWriter::JsonStreamWriter()
{
    mGenerator = yajl_gen_alloc(NULL);  
    //yajl_gen_config(mGenerator, yajl_gen_beautify, 1);  
    //yajl_gen_config(mGenerator, yajl_gen_validate_utf8, 1);
}

JsonStreamWriter::~JsonStreamWriter()
{
    if (mGenerator) {
        yajl_gen_free(mGenerator);
        mGenerator = NULL;
    }
}

bool JsonStreamWriter::writeNull()
{
    return yajl_gen_status_ok == yajl_gen_null(mGenerator);
}

bool JsonStreamWriter::writeBool(bool boolean)
{
    return yajl_gen_status_ok == yajl_gen_bool(mGenerator, boolean);
}

bool JsonStreamWriter::writeInteger(long long number)
{
    return yajl_gen_status_ok == yajl_gen_integer(mGenerator, number);
}

bool JsonStreamWriter::writeDouble(double number)
{
    return yajl_gen_status_ok == yajl_gen_double(mGenerator, number);
}

bool JsonStreamWriter::writeString(const QString & string)
{
    QByteArray utf8 = string.toUtf8();
    return yajl_gen_status_ok == yajl_gen_string(mGenerator, (const unsigned char*)utf8.data(), utf8.length());
}

bool JsonStreamWriter::writeKey(const QString & key)
{
    return writeString(key);
}

bool JsonStreamWriter::writeStartObject()
{
    return yajl_gen_status_ok == yajl_gen_map_open(mGenerator); 
}

bool JsonStreamWriter::writeEndObject()
{
    return yajl_gen_status_ok == yajl_gen_map_close(mGenerator);
}

bool JsonStreamWriter::writeStartArray()
{
    return yajl_gen_status_ok == yajl_gen_array_open(mGenerator);
}

bool JsonStreamWriter::writeEndArray()
{
    return yajl_gen_status_ok == yajl_gen_array_close(mGenerator);
}

QByteArray JsonStreamWriter::data()
{
    const unsigned char * buf;  
    size_t len;  
    yajl_gen_get_buf(mGenerator, &buf, &len);  
    
    return QByteArray((const char *)buf, len);
     
    //yajl_gen_clear(g);
}
