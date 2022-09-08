/****************************************************************************
**
** Copyright (C) 2015 Klaralvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
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
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtTest/QTest>
#include <Qt3DCore/private/qnode_p.h>
#include <Qt3DCore/private/qscene_p.h>

#include <Qt3DRender/qclearbuffers.h>
#include <Qt3DRender/private/qclearbuffers_p.h>
#include <testarbiter.h>

class tst_QClearBuffers: public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void checkPropertyUpdates()
    {
        // GIVEN
        TestArbiter arbiter;
        QScopedPointer<Qt3DRender::QClearBuffers> clearBuffer(new Qt3DRender::QClearBuffers());
        arbiter.setArbiterOnNode(clearBuffer.data());

        // WHEN
        clearBuffer->setBuffers(Qt3DRender::QClearBuffers::AllBuffers);

        // THEN
        QCOMPARE(arbiter.dirtyNodes().size(), 1);
        QCOMPARE(arbiter.dirtyNodes().front(), clearBuffer.data());

        arbiter.clear();

        // WHEN
        clearBuffer->setBuffers(Qt3DRender::QClearBuffers::AllBuffers);

        // THEN
        QCOMPARE(arbiter.dirtyNodes().size(), 0);

        // WHEN
        clearBuffer->setBuffers(Qt3DRender::QClearBuffers::ColorDepthBuffer);

        // THEN
        QCOMPARE(arbiter.dirtyNodes().size(), 1);
        QCOMPARE(arbiter.dirtyNodes().front(), clearBuffer.data());

        arbiter.clear();
    }
};

QTEST_MAIN(tst_QClearBuffers)

#include "tst_qclearbuffers.moc"