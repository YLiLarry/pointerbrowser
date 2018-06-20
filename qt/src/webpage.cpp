#include "webpage.h"
#include <QString>
#include <QVariantMap>
#include <QSharedPointer>
#include <QJsonObject>
#include <QQmlEngine>

#define WP_PROP_1 (QString, title)

#define QPROP_FUNC(TYPE, PROP) \
    TYPE Webpage::PROP() const { return _##PROP; } \
    void Webpage::set_##PROP(TYPE x) { _##PROP = x; emit PROP##_changed(x); }

QPROP_FUNC(QString, title)
QPROP_FUNC(QString, html)
QPROP_FUNC(QString, url)
QPROP_FUNC(QString, hash)
QPROP_FUNC(QString, symbol)
QPROP_FUNC(QString, display)
QPROP_FUNC(QString, expanded_display)
QPROP_FUNC(quint64, visited)
QPROP_FUNC(bool, preview_mode)
QPROP_FUNC(bool, url_matched)
QPROP_FUNC(bool, title_matched)
QPROP_FUNC(bool, hash_matched)
QPROP_FUNC(bool, symbol_matched)

Webpage::Webpage(QString url)
{
    _url = url;
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}


QVariantMap Webpage::toQVariantMap()
{
    QVariantMap map;
#define MAP_INSERT(NAME) map.insert(#NAME, NAME())
    MAP_INSERT(title);
    MAP_INSERT(url);
    MAP_INSERT(html);
    MAP_INSERT(visited);
    MAP_INSERT(hash);
    MAP_INSERT(symbol);
    MAP_INSERT(display);
    MAP_INSERT(expanded_display);
    MAP_INSERT(url_matched);
    MAP_INSERT(title_matched);
    MAP_INSERT(hash_matched);
    MAP_INSERT(symbol_matched);
    return map;
}

Webpage_ Webpage::fromQVariantMap(QVariantMap& map)
{
    Webpage_ webpage = Webpage_::create(map["url"].value<QString>());
    webpage->set_title(map["title"].value<QString>());
    return webpage;
}

QJsonObject Webpage::toQJsonObject()
{
    QJsonObject map;
    map.insert("title", title());
    map.insert("url", url());
    return map;
}

Webpage_ Webpage::fromQJsonObject(QJsonObject& map)
{
    Webpage_ webpage = QSharedPointer<Webpage>::create(map["url"].toString());
    webpage->set_title(map["title"].toString());
    return webpage;
}

//Webpage_ Webpage::create(const QString& url)
//{
//    Webpage_ webpage = QSharedPointer<Webpage>::create(url);
//    return webpage;
//}
