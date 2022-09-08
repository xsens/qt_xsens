/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Copyright (C) 2016 The Qt Company Ltd and/or its subsidiary(-ies).
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

#ifndef QT3DRENDER_RENDER_RENDERSTATE_H
#define QT3DRENDER_RENDER_RENDERSTATE_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <Qt3DRender/private/genericstate_p.h>
#include <Qt3DRender/private/renderstates_p.h>
#include <Qt3DRender/private/statevariant_p.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

class QRenderState;

namespace Render {

class RenderState;

class Q_3DRENDERSHARED_PRIVATE_EXPORT RenderStateSet
{
public:
    RenderStateSet();
    ~RenderStateSet();

    template<typename GenericState>
    void addState(const GenericState &state)
    {
        addState(StateVariant::fromValue(state));
    }

    /**
     * @brief changeCost - metric of cost to change to this state-set from
     * a candidate previous state-set. This is used to find an optimal
     * ordering of state-sets when sending draw commands.
     * @param previousState
     * @return
     */
    int changeCost(RenderStateSet* previousState);

    StateMaskSet stateMask() const;
    void merge(const RenderStateSet *other);

    const std::vector<StateVariant>& states() const noexcept { return m_states; }
    std::vector<StateVariant>& states() noexcept { return m_states; }

    bool canAddStateOfType(StateMask type) const;

    /**
     * @brief contains - check if this set contains a matching piece of state
     * @param ds
     * @return
     */
    bool contains(const StateVariant &ds) const;

private:
    bool hasStateOfType(StateMask type) const;
    bool allowMultipleStatesOfType(StateMask type) const;

private:
    StateMaskSet m_stateMask;
    std::vector<StateVariant> m_states;
};

template<>
Q_3DRENDERSHARED_PRIVATE_EXPORT void RenderStateSet::addState<StateVariant>(const StateVariant &state);

} // namespace Render
} // namespace Qt3DRender

QT_END_NAMESPACE

#endif // QT3DRENDER_RENDER_RENDERSTATE_H
