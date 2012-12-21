#ifndef JSONWRITER_H
#define JSONWRITER_H

#include <QtCore>
#include "jsonstreamwriter.h"


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

#endif // JSONWRITER_H
