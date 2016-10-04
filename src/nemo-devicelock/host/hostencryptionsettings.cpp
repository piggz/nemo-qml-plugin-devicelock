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

#include "hostencryptionsettings.h"

namespace NemoDeviceLock
{

HostEncryptionSettingsAdaptor::HostEncryptionSettingsAdaptor(HostEncryptionSettings *settings)
    : QDBusAbstractAdaptor(settings)
    , m_settings(settings)
{
}

bool HostEncryptionSettingsAdaptor::isSupported() const
{
    return m_settings->isSupported();
}

void HostEncryptionSettingsAdaptor::EncryptHome(
        const QDBusObjectPath &path, const QDBusVariant &authenticationToken)
{
    m_settings->encryptHome(path.path(), authenticationToken.variant());
}

HostEncryptionSettings::HostEncryptionSettings(QObject *parent)
    : HostEncryptionSettings(Authenticator::Methods(), parent)
{
}

HostEncryptionSettings::HostEncryptionSettings(Authenticator::Methods allowedMethods, QObject *parent)
    : HostAuthorization(QStringLiteral("/encryption"), allowedMethods, parent)
    , m_adaptor(this)
{
}

HostEncryptionSettings::~HostEncryptionSettings()
{
}

bool HostEncryptionSettings::isSupported() const
{
    return false;
}

void HostEncryptionSettings::encryptHome(const QString &, const QVariant &)
{
    QDBusContext::sendErrorReply(QDBusError::NotSupported);
}

}
