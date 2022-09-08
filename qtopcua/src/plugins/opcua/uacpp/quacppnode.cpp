/******************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtOpcUa module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#include "quacppbackend.h"
#include "quacppnode.h"
#include "quacpputils.h"

#include <QtCore/QDateTime>
#include <QtCore/QLoggingCategory>

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(QT_OPCUA_PLUGINS_UACPP)

QUACppNode::QUACppNode(const UaNodeId nodeId, QUACppClient *client, const QString nodeIdString)
    : m_client(client)
    , m_nodeIdString(nodeIdString)
    , m_nodeId(nodeId)
    , m_handle(0)
{
    bool success = m_client->registerNode(this);
    setRegistered(success);
}

QUACppNode::~QUACppNode()
{
    if (m_client)
        m_client->unregisterNode(this);
}

bool QUACppNode::readAttributes(QOpcUa::NodeAttributes attr, const QString &indexRange)
{
    if (!m_client)
        return false;

    return QMetaObject::invokeMethod(m_client->m_backend, "readAttributes",
                                     Qt::QueuedConnection,
                                     Q_ARG(quint64, handle()),
                                     Q_ARG(UaNodeId, m_nodeId),
                                     Q_ARG(QOpcUa::NodeAttributes, attr),
                                     Q_ARG(QString, indexRange));
}

bool QUACppNode::enableMonitoring(QOpcUa::NodeAttributes attr, const QOpcUaMonitoringParameters &settings)
{
    if (!m_client)
        return false;

    return QMetaObject::invokeMethod(m_client->m_backend, "enableMonitoring",
                                     Qt::QueuedConnection,
                                     Q_ARG(quint64, handle()),
                                     Q_ARG(UaNodeId, m_nodeId),
                                     Q_ARG(QOpcUa::NodeAttributes, attr),
                                     Q_ARG(QOpcUaMonitoringParameters, settings));
}

bool QUACppNode::disableMonitoring(QOpcUa::NodeAttributes attr)
{
    if (!m_client)
        return false;

    return QMetaObject::invokeMethod(m_client->m_backend, "disableMonitoring",
                                     Qt::QueuedConnection,
                                     Q_ARG(quint64, handle()),
                                     Q_ARG(QOpcUa::NodeAttributes, attr));
}

bool QUACppNode::modifyMonitoring(QOpcUa::NodeAttribute attr, QOpcUaMonitoringParameters::Parameter item, const QVariant &value)
{
    if (!m_client)
        return false;

    return QMetaObject::invokeMethod(m_client->m_backend, "modifyMonitoring",
                                     Qt::QueuedConnection,
                                     Q_ARG(quint64, handle()),
                                     Q_ARG(QOpcUa::NodeAttribute, attr),
                                     Q_ARG(QOpcUaMonitoringParameters::Parameter, item),
                                     Q_ARG(QVariant, value));
}

bool QUACppNode::browse(const QOpcUaBrowseRequest &request)
{
    if (!m_client)
        return false;

    return QMetaObject::invokeMethod(m_client->m_backend, "browse",
                                     Qt::QueuedConnection,
                                     Q_ARG(quint64, handle()),
                                     Q_ARG(UaNodeId, m_nodeId),
                                     Q_ARG(QOpcUaBrowseRequest, request));
}

QString QUACppNode::nodeId() const
{
    return m_nodeIdString;
}

bool QUACppNode::writeAttribute(QOpcUa::NodeAttribute attribute, const QVariant &value, QOpcUa::Types type, const QString &indexRange)
{
    if (!m_client)
        return false;

    return QMetaObject::invokeMethod(m_client->m_backend, "writeAttribute",
                                     Qt::QueuedConnection,
                                     Q_ARG(quint64, handle()),
                                     Q_ARG(UaNodeId, m_nodeId),
                                     Q_ARG(QOpcUa::NodeAttribute, attribute),
                                     Q_ARG(QVariant, value),
                                     Q_ARG(QOpcUa::Types, type),
                                     Q_ARG(QString, indexRange));
}

bool QUACppNode::writeAttributes(const QOpcUaNode::AttributeMap &toWrite, QOpcUa::Types valueAttributeType)
{
    if (!m_client)
        return false;

    return QMetaObject::invokeMethod(m_client->m_backend, "writeAttributes",
                                     Qt::QueuedConnection,
                                     Q_ARG(quint64, handle()),
                                     Q_ARG(UaNodeId, m_nodeId),
                                     Q_ARG(QOpcUaNode::AttributeMap, toWrite),
                                     Q_ARG(QOpcUa::Types, valueAttributeType));

}

bool QUACppNode::callMethod(const QString &methodNodeId, const QList<QOpcUa::TypedVariant> &args)
{
    if (!m_client)
        return false;

    const UaNodeId methodId = UACppUtils::nodeIdFromQString(methodNodeId);

    return QMetaObject::invokeMethod(m_client->m_backend, "callMethod",
                                     Qt::QueuedConnection,
                                     Q_ARG(quint64, handle()),
                                     Q_ARG(UaNodeId, m_nodeId),
                                     Q_ARG(UaNodeId, methodId),
                                     Q_ARG(QList<QOpcUa::TypedVariant>, args));
}

bool QUACppNode::resolveBrowsePath(const QList<QOpcUaRelativePathElement> &path)
{
    if (!m_client)
        return false;

    return QMetaObject::invokeMethod(m_client->m_backend, "resolveBrowsePath",
                                     Qt::QueuedConnection,
                                     Q_ARG(quint64, handle()),
                                     Q_ARG(UaNodeId, m_nodeId),
                                     Q_ARG(QList<QOpcUaRelativePathElement>, path));
}

QT_END_NAMESPACE
