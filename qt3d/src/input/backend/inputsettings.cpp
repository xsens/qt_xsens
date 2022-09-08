/****************************************************************************
**
** Copyright (C) 2016 Klaralvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "inputsettings_p.h"

#include <Qt3DInput/qinputsettings.h>

#include <Qt3DInput/private/inputhandler_p.h>
#include <Qt3DInput/private/qinputsettings_p.h>

QT_BEGIN_NAMESPACE

namespace Qt3DInput {

namespace Input {

InputSettings::InputSettings()
    : BackendNode(QBackendNode::ReadOnly)
    , m_eventSource(nullptr)
{
}

void InputSettings::syncFromFrontEnd(const Qt3DCore::QNode *frontEnd, bool firstTime)
{
    BackendNode::syncFromFrontEnd(frontEnd, firstTime);
    const QInputSettings *node = qobject_cast<const QInputSettings *>(frontEnd);
    if (!node)
        return;

    m_eventSource = node->eventSource();
}

InputSettingsFunctor::InputSettingsFunctor(InputHandler *handler)
    : m_handler(handler)
{
}

Qt3DCore::QBackendNode *InputSettingsFunctor::create(Qt3DCore::QNodeId id) const
{
    Q_UNUSED(id);
    if (m_handler->inputSettings() != nullptr) {
        qWarning() << "Input settings already specified";
        return nullptr;
    }

    InputSettings *settings = new InputSettings();
    m_handler->setInputSettings(settings);
    return settings;
}

Qt3DCore::QBackendNode *InputSettingsFunctor::get(Qt3DCore::QNodeId id) const
{
    InputSettings *settings = m_handler->inputSettings();
    if (settings != nullptr && settings->peerId() == id)
        return settings;
    return nullptr;
}

void InputSettingsFunctor::destroy(Qt3DCore::QNodeId id) const
{
    InputSettings *settings = m_handler->inputSettings();
    if (settings != nullptr && settings->peerId() == id) {
        m_handler->setInputSettings(nullptr);
        delete settings;
    }
}

} // Input

} // Qt3DInput

QT_END_NAMESPACE
