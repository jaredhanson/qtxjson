#ifndef JSONREADER_H
#define JSONREADER_H

#include "jsonglobal.h"
#include <QtCore>
#include "jsonstreamreader.h"

QTX_BEGIN_NAMESPACE


class JsonReaderContext;

class JsonReader : public QObject
{
    Q_OBJECT

public:
    static QHash<QString, QVariant> parse(const QByteArray & data);

public:
    JsonReader(QObject * parent = 0);
    ~JsonReader();
        
    void addData(const QByteArray & data);
    void parse();
    
    QString errorString() const;
    
signals:
    void objectDecoded(const QVariantHash & object);
    void error(JsonStreamReader::Error err);
        
private:
    void processStartObject();
    void processEndObject();
    void processStartArray();
    void processEndArray();
    void processValue();
    void processInvalid();
        
private:
    JsonStreamReader mReader;
    QStack<JsonReaderContext *> mContexts;
    
    // hacky ivar to support static parse function.  clean this up.
    bool mKeepRootContext;
    JsonReaderContext* mRootContext;
    
    QString mErrorString;
};

class JsonReaderContext
{
public:
    typedef enum {
        Object,
        Array
    } Type;
    
public:
    JsonReaderContext(const QString & key, const QHash<QString, QVariant> & object);
    JsonReaderContext(const QString & key, const QList<QVariant> & array);
    ~JsonReaderContext();
    
    Type type() const;
    QString key() const;
    QHash<QString, QVariant> object() const;
    QList<QVariant> array() const;
    QVariant value() const;
    
    bool add(const QString & key, const QVariant & value);
    
private:
    Type mType;
    QString mKey;
    QHash<QString, QVariant> mObject;
    QList<QVariant> mArray;
};


QTX_END_NAMESPACE

#endif // JSONREADER_H
