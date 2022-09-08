/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the ActiveQt framework of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QAXBASE_H
#define QAXBASE_H

#include <QtCore/qdatastream.h>
#include <QtCore/qmap.h>
#include <QtCore/qobject.h>
#include <QtCore/qvariant.h>

struct IUnknown;
struct IDispatch;

QT_BEGIN_NAMESPACE

class QUuid;
class QAxEventSink;
class QAxObject;
class QAxBasePrivate;

class QAxBase
{
public:
    using PropertyBag = QMap<QString, QVariant>;

    virtual ~QAxBase();

    QString control() const;

    long queryInterface(const QUuid &, void**) const;

    QVariant dynamicCall(const char *name, const QVariant &v1 = QVariant(),
                                           const QVariant &v2 = QVariant(),
                                           const QVariant &v3 = QVariant(),
                                           const QVariant &v4 = QVariant(),
                                           const QVariant &v5 = QVariant(),
                                           const QVariant &v6 = QVariant(),
                                           const QVariant &v7 = QVariant(),
                                           const QVariant &v8 = QVariant());
    QVariant dynamicCall(const char *name, QList<QVariant> &vars);
    QAxObject *querySubObject(const char *name, const QVariant &v1 = QVariant(),
                                           const QVariant &v2 = QVariant(),
                                           const QVariant &v3 = QVariant(),
                                           const QVariant &v4 = QVariant(),
                                           const QVariant &v5 = QVariant(),
                                           const QVariant &v6 = QVariant(),
                                           const QVariant &v7 = QVariant(),
                                           const QVariant &v8 = QVariant());
    QAxObject* querySubObject(const char *name, QList<QVariant> &vars);

    const QMetaObject *axBaseMetaObject() const;

    const char *className() const;
    QObject *qObject() const;

    PropertyBag propertyBag() const;
    void setPropertyBag(const PropertyBag&);

    QString generateDocumentation();

    virtual bool propertyWritable(const char*) const;
    virtual void setPropertyWritable(const char*, bool);

    bool isNull() const;

    QStringList verbs() const;

    QVariant asVariant() const;

public:
    void clear();
    bool setControl(const QString&);

    void disableMetaObject();
    void disableClassInfo();
    void disableEventSink();

    ulong classContext() const;
    void setClassContext(ulong classContext);

protected:
    QAxBase();

    virtual bool initialize(IUnknown** ptr);
    bool initializeRemote(IUnknown** ptr);
    bool initializeLicensed(IUnknown** ptr);
    bool initializeActive(IUnknown** ptr);
    bool initializeFromFile(IUnknown** ptr);

    void internalRelease();
    void initializeFrom(QAxBase *that);
    void connectNotify();
    long indexOfVerb(const QString &verb) const;
    QVariant dynamicCall(const char *name, QList<QVariant> &vars, unsigned flags);
    static QVariantList argumentsToList(const QVariant &var1, const QVariant &var2,
                                        const QVariant &var3, const QVariant &var4,
                                        const QVariant &var5, const QVariant &var6,
                                        const QVariant &var7, const QVariant &var8);

    void axBaseInit(QAxBasePrivate *b, IUnknown *iface = nullptr);

private:
    enum DynamicCallHelperFlags {
        NoPropertyGet = 0x1 // Suppresses DISPATCH_PROPERTYGET, use for plain functions.
    };

    friend class QAxScript;
    friend class QAxEventSink;
    friend class QAxBasePrivate;
    friend void *qax_createObjectWrapper(int, IUnknown*);
    bool initializeLicensedHelper(void *factory, const QString &key, IUnknown **ptr);
    QAxBasePrivate *d = nullptr;

    int internalProperty(QMetaObject::Call, int index, void **v);
    int internalInvoke(QMetaObject::Call, int index, void **v);
    bool dynamicCallHelper(const char *name, void *out, QList<QVariant> &var,
                           QByteArray &type, unsigned flags = 0);
};

template <> inline QAxBase *qobject_cast<QAxBase*>(const QObject *o)
{
    void *result = o ? const_cast<QObject *>(o)->qt_metacast("QAxBase") : nullptr;
    return static_cast<QAxBase *>(result);
}

template <> inline QAxBase *qobject_cast<QAxBase*>(QObject *o)
{
    void *result = o ? o->qt_metacast("QAxBase") : nullptr;
    return static_cast<QAxBase *>(result);
}

extern QString qax_generateDocumentation(QAxBase *);

inline QString QAxBase::generateDocumentation()
{
    return qax_generateDocumentation(this);
}

#ifndef QT_NO_DATASTREAM
inline QDataStream &operator >>(QDataStream &s, QAxBase &c)
{
    QAxBase::PropertyBag bag;
    const QSignalBlocker blocker(c.qObject());
    QString control;
    s >> control;
    c.setControl(control);
    s >> bag;
    c.setPropertyBag(bag);

    return s;
}

inline QDataStream &operator <<(QDataStream &s, const QAxBase &c)
{
    QAxBase::PropertyBag bag = c.propertyBag();
    s << c.control();
    s << bag;

    return s;
}
#endif // QT_NO_DATASTREAM

QT_END_NAMESPACE

#ifndef Q_COM_METATYPE_DECLARED
#define Q_COM_METATYPE_DECLARED

Q_DECLARE_OPAQUE_POINTER(IUnknown*)
Q_DECLARE_OPAQUE_POINTER(IDispatch*)

Q_DECLARE_METATYPE(IUnknown*)
Q_DECLARE_METATYPE(IDispatch*)

#endif

#endif // QAXBASE_H