#include "json.h"
#include "jsonreader.h"

QTX_BEGIN_NAMESPACE


QHash<QString, QVariant> Json::parse(const QByteArray & data)
{
    Q_UNUSED(data)
    
    //qDebug() << "Static parse data";
    
    JsonReader reader;
    reader.mKeepRootContext = true;
    reader.addData(data);
    reader.parse();
    
    // @todo: should check for errors at this point.
    
    if (reader.mRootContext) {
        return reader.mRootContext->object();
    }
    
    return QHash<QString, QVariant>();
}

QString JsonWriter::stringify(const QHash<QString, QVariant> & object)
{
    JsonWriter writer;
    QByteArray bytes = writer.write(object);
    return QString::fromUtf8(bytes);
}
