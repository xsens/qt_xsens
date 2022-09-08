/****************************************************************************
**
** Copyright (C) 2016 Paul Lemire
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

#include "lightgatherer_p.h"
#include <Qt3DRender/private/job_common_p.h>
#include <Qt3DRender/private/managers_p.h>
#include <Qt3DRender/private/entity_p.h>
#include <Qt3DRender/private/qt3drender_global_p.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

namespace Render {

LightGatherer::LightGatherer()
    : Qt3DCore::QAspectJob()
    , m_manager(nullptr)
    , m_environmentLight(nullptr)
{
    SET_JOB_RUN_STAT_TYPE(this, JobTypes::LightGathering, 0)
}

void LightGatherer::run()
{
    m_lights.clear();
    m_environmentLight = nullptr;

    const std::vector<HEntity> &handles = m_manager->activeHandles();
    size_t envLightCount = 0;

    for (const HEntity &handle : handles) {
        Entity *node = m_manager->data(handle);
        std::vector<Light *> lights = node->renderComponents<Light>();
        if (!lights.empty())
            m_lights.push_back(LightSource(node, std::move(lights)));
        const std::vector<EnvironmentLight *> &envLights = node->renderComponents<EnvironmentLight>();
        envLightCount += envLights.size();
        if (!envLights.empty() && !m_environmentLight)
            m_environmentLight = envLights.front();
    }

    if (envLightCount > 1)
        qWarning() << "More than one environment light found, extra instances are ignored";
}

} // Render

} // Qt3DRender

QT_END_NAMESPACE