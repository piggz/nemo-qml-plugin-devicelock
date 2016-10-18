/*
 * Copyright (C) 2016 Jolla Ltd
 * Contact: Andrew den Exter <andrew.den.exter@jolla.com>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Nemo Mobile nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#include "hostdevicelock.h"

#include "settingswatcher.h"

namespace NemoDeviceLock
{

HostDeviceLockAdaptor::HostDeviceLockAdaptor(HostDeviceLock *deviceLock)
    : QDBusAbstractAdaptor(deviceLock)
    , m_deviceLock(deviceLock)
{
}

uint HostDeviceLockAdaptor::state() const
{
    return m_deviceLock->state();
}

bool HostDeviceLockAdaptor::isEnabled() const
{
    return m_deviceLock->isEnabled();
}

bool HostDeviceLockAdaptor::isUnlocking() const
{
    return m_deviceLock->isUnlocking();
}

void HostDeviceLockAdaptor::Unlock()
{
    m_deviceLock->unlock();
}

void HostDeviceLockAdaptor::Cancel()
{
    m_deviceLock->cancel();
}

HostDeviceLock::HostDeviceLock(Authenticator::Methods supportedMethods, QObject *parent)
    : HostAuthenticationInput(QStringLiteral("/devicelock/lock"), supportedMethods, parent)
    , m_adaptor(this)
    , m_settings(SettingsWatcher::instance())
{
    connect(m_settings.data(), &SettingsWatcher::automaticLockingChanged,
            this, &HostDeviceLock::automaticLockingChanged);
}

HostDeviceLock::~HostDeviceLock()
{
}

int HostDeviceLock::automaticLocking() const
{
    return isEnabled() ? m_settings->automaticLocking : -1;
}

void HostDeviceLock::stateChanged()
{
    propertyChanged(
                QStringLiteral("org.nemomobile.devicelock.DeviceLock"),
                QStringLiteral("State"),
                QVariant::fromValue(uint(state())));
}

void HostDeviceLock::enabledChanged()
{
    propertyChanged(
                QStringLiteral("org.nemomobile.devicelock.DeviceLock"),
                QStringLiteral("Enabled"),
                isEnabled());
}

void HostDeviceLock::unlockingChanged()
{
    propertyChanged(
                QStringLiteral("org.nemomobile.devicelock.DeviceLock"),
                QStringLiteral("Unlocking"),
                isUnlocking());
}

void HostDeviceLock::automaticLockingChanged()
{
}

}
