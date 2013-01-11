#ifndef JSONSTREAMWRITER_H
#define JSONSTREAMWRITER_H

#include "jsonglobal.h"
#include <QtCore>
#include <yajl/yajl_gen.h>

QTX_BEGIN_NAMESPACE


class JsonStreamWriter
{
public:
    JsonStreamWriter();
    ~JsonStreamWriter();

    bool writeNull();
    bool writeBool(bool boolean);
    bool writeInteger(long long number);
    bool writeDouble(double number);
    bool writeString(const QString & string);
    bool writeKey(const QString & key);
    bool writeStartObject();
    bool writeEndObject();
    bool writeStartArray();
    bool writeEndArray();
    
    QByteArray data();
    
private:
    yajl_gen mGenerator;
};


QTX_END_NAMESPACE

#endif // JSONSTREAMWRITER_H
