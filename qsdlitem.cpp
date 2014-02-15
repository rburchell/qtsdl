#include "qsdlitem.h"
#include <QQmlInfo>

QSdlItem::QSdlItem(QObject *parent)
    : QObject(parent)
    , m_window(0)
{
}

QSdlItem::~QSdlItem()
{
}

QSdlWindow *QSdlItem::window() const
{
    qDebug() << m_window;
    return m_window;
}

void QSdlItem::render()
{
    qDebug() << "Render for " << this;
    foreach (QObject *child, children()) {
        QSdlItem *item = qobject_cast<QSdlItem *>(child);
        if (!item)
            continue;
        item->render();
    }
}


QQmlListProperty<QObject> QSdlItem::data()
{
    return QQmlListProperty<QObject>(this, 0, QSdlItem::data_append,
                                             QSdlItem::data_count,
                                             QSdlItem::data_at,
                                             QSdlItem::data_clear);
}

void QSdlItem::data_append(QQmlListProperty<QObject> *prop, QObject *object)
{
    QSdlItem *fo = qobject_cast<QSdlItem *>(object);
    if (!fo) {
        qWarning("What type of item is this? Not a QSdlItem!");
        return;
    }

    QSdlItem *that = static_cast<QSdlItem *>(prop->object);
    qDebug() << "appending " << fo << " to " << that;
    fo->setParent(that);
    fo->m_window = that->m_window;
}

int QSdlItem::data_count(QQmlListProperty<QObject> *prop)
{
    // TODO: don't abuse children for this
    QSdlItem *that = static_cast<QSdlItem *>(prop->object);
    return that->children().count();
}

QObject *QSdlItem::data_at(QQmlListProperty<QObject> *prop, int idx)
{
    QSdlItem *that = static_cast<QSdlItem *>(prop->object);
    return that->children().at(idx);
}

void QSdlItem::data_clear(QQmlListProperty<QObject> *prop)
{
    QSdlItem *that = static_cast<QSdlItem *>(prop->object);
    foreach (QObject *obj, that->children()) {
        obj->setParent(0);
    }
}

