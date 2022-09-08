/****************************************************************************
**
** Copyright (C) 2016 Klaralvdalens Datakonsult AB (KDAB).
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
#include <qbackendnodetester.h>
#include "testdevice.h"

#include <Qt3DInput/private/actioninput_p.h>
#include <Qt3DInput/private/inputhandler_p.h>
#include <Qt3DInput/QActionInput>

class tst_ActionInput : public Qt3DCore::QBackendNodeTester
{
    Q_OBJECT
private Q_SLOTS:
    void shouldMirrorPeerProperties()
    {
        // GIVEN
        Qt3DInput::Input::ActionInput backendActionInput;
        Qt3DInput::QActionInput actionInput;
        TestDevice sourceDevice;

        actionInput.setButtons(QList<int> { 1 << 8 });
        actionInput.setSourceDevice(&sourceDevice);

        // WHEN
        simulateInitializationSync(&actionInput, &backendActionInput);

        // THEN
        QCOMPARE(backendActionInput.peerId(), actionInput.id());
        QCOMPARE(backendActionInput.isEnabled(), actionInput.isEnabled());
        QCOMPARE(backendActionInput.buttons(), actionInput.buttons());
        QCOMPARE(backendActionInput.sourceDevice(), sourceDevice.id());
    }

    void shouldHaveInitialAndCleanedUpStates()
    {
        // GIVEN
        Qt3DInput::Input::ActionInput backendActionInput;

        // THEN
        QVERIFY(backendActionInput.peerId().isNull());
        QVERIFY(backendActionInput.buttons().isEmpty());
        QCOMPARE(backendActionInput.isEnabled(), false);
        QCOMPARE(backendActionInput.sourceDevice(), Qt3DCore::QNodeId());

        // GIVEN
        Qt3DInput::QActionInput actionInput;
        TestDevice sourceDevice;

        actionInput.setButtons(QList<int> { 1 << 8 });
        actionInput.setSourceDevice(&sourceDevice);

        // WHEN
        simulateInitializationSync(&actionInput, &backendActionInput);
        backendActionInput.cleanup();

        // THEN
        QVERIFY(backendActionInput.buttons().isEmpty());
        QCOMPARE(backendActionInput.isEnabled(), false);
        QCOMPARE(backendActionInput.sourceDevice(), Qt3DCore::QNodeId());
    }

    void shouldHandlePropertyChanges()
    {
        // GIVEN
        Qt3DInput::QActionInput actionInput;
        Qt3DInput::Input::ActionInput backendActionInput;
        simulateInitializationSync(&actionInput, &backendActionInput);

        // WHEN
        actionInput.setButtons(QList<int> { 64 });
        backendActionInput.syncFromFrontEnd(&actionInput, false);

        // THEN
        QCOMPARE(backendActionInput.buttons(), QList<int> { 64 });

        // WHEN
        actionInput.setEnabled(false);
        backendActionInput.syncFromFrontEnd(&actionInput, false);

        // THEN
        QCOMPARE(backendActionInput.isEnabled(), false);

        // WHEN
        TestDevice device;
        actionInput.setSourceDevice(&device);
        backendActionInput.syncFromFrontEnd(&actionInput, false);

        // THEN
        QCOMPARE(backendActionInput.sourceDevice(), device.id());
    }

    void shouldDealWithKeyPresses()
    {
        // GIVEN
        TestDeviceIntegration deviceIntegration;
        TestDevice *device = deviceIntegration.createPhysicalDevice("keyboard");
        TestDeviceBackendNode *deviceBackend = deviceIntegration.physicalDevice(device->id());
        Qt3DInput::Input::InputHandler handler;
        handler.addInputDeviceIntegration(&deviceIntegration);

        Qt3DInput::Input::ActionInput backendActionInput;
        Qt3DInput::QActionInput actionInput;
        actionInput.setEnabled(true);
        actionInput.setButtons(QList<int> { Qt::Key_Space, Qt::Key_Return });
        actionInput.setSourceDevice(device);
        simulateInitializationSync(&actionInput, &backendActionInput);

        // WHEN
        deviceBackend->setButtonPressed(Qt::Key_Up, true);

        // THEN
        QCOMPARE(backendActionInput.process(&handler, 1000000000), false);

        // WHEN
        deviceBackend->setButtonPressed(Qt::Key_Space, true);

        // THEN
        QCOMPARE(backendActionInput.process(&handler, 1000000000), true);

        // WHEN
        deviceBackend->setButtonPressed(Qt::Key_Return, true);

        // THEN
        QCOMPARE(backendActionInput.process(&handler, 1000000000), true);

        // WHEN
        deviceBackend->setButtonPressed(Qt::Key_Space, false);

        // THEN
        QCOMPARE(backendActionInput.process(&handler, 1000000000), true);

        // WHEN
        deviceBackend->setButtonPressed(Qt::Key_Up, false);

        // THEN
        QCOMPARE(backendActionInput.process(&handler, 1000000000), true);

        // WHEN
        deviceBackend->setButtonPressed(Qt::Key_Return, false);

        // THEN
        QCOMPARE(backendActionInput.process(&handler, 1000000000), false);
    }

    void shouldNotProcessWhenDisabled()
    {
        // GIVEN
        TestDeviceIntegration deviceIntegration;
        TestDevice *device = deviceIntegration.createPhysicalDevice("keyboard");
        TestDeviceBackendNode *deviceBackend = deviceIntegration.physicalDevice(device->id());
        Qt3DInput::Input::InputHandler handler;
        handler.addInputDeviceIntegration(&deviceIntegration);

        Qt3DInput::Input::ActionInput backendActionInput;
        Qt3DInput::QActionInput actionInput;
        actionInput.setEnabled(false);
        actionInput.setButtons(QList<int> { Qt::Key_Space, Qt::Key_Return });
        actionInput.setSourceDevice(device);
        simulateInitializationSync(&actionInput, &backendActionInput);

        // WHEN
        deviceBackend->setButtonPressed(Qt::Key_Space, true);

        // THEN
        QCOMPARE(backendActionInput.process(&handler, 1000000000), false);
    }
};

QTEST_APPLESS_MAIN(tst_ActionInput)

#include "tst_actioninput.moc"
