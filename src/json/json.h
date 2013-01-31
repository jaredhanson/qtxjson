#ifndef QTXJSON_JSON_H
#define QTXJSON_JSON_H

#include "jsonglobal.h"
#include <QtCore>

QTX_BEGIN_NAMESPACE


class Json {
public:
    static QHash<QString, QVariant> parse(const QByteArray & data);
    static QString stringify(const QHash<QString, QVariant> & object);
};


QTX_END_NAMESPACE

#endif // QTXJSON_JSON_H
