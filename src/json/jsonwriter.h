#ifndef JSONWRITER_H
#define JSONWRITER_H

#include "jsonglobal.h"
#include <QtCore>
#include "jsonstreamwriter.h"

QTX_BEGIN_NAMESPACE


class JsonWriter
{
public:
    static QString stringify(const QHash<QString, QVariant> & object);
    
public:
    JsonWriter();
    ~JsonWriter();

    QByteArray write(const QHash<QString, QVariant> & object);
    
protected:
    void writeValue(const QVariant & value);
    void writeObject(const QHash<QString, QVariant> & object);
    void writeArray(const QList<QVariant> & array);
    
private:
    JsonStreamWriter mWriter;
};


QTX_END_NAMESPACE

#endif // JSONWRITER_H
