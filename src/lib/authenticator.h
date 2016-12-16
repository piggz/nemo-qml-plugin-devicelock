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

#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include <QDBusAbstractAdaptor>
#include <QDBusObjectPath>
#include <QSharedDataPointer>

#include <connection.h>

namespace NemoDeviceLock {

class Authenticator;
class AuthenticatorAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.nemomobile.devicelock.client.Authenticator")
public:
    explicit AuthenticatorAdaptor(Authenticator *authenticator);

public slots:
    Q_NOREPLY void Authenticated(const QDBusVariant &authenticationToken);
    Q_NOREPLY void Feedback(uint feedback, uint attemptsRemaining, uint utilizedMethods);
    Q_NOREPLY void Error(uint error);

private:
    Authenticator *m_authenticator;
};

class SettingsWatcher;
class Authenticator : public QObject, private ConnectionClient
{
    Q_OBJECT
    Q_PROPERTY(Methods availableMethods READ availableMethods NOTIFY availableMethodsChanged)
    Q_PROPERTY(Methods utilizedMethods READ utilizedMethods NOTIFY utilizedMethodsChanged)
    Q_PROPERTY(bool authenticating READ isAuthenticating NOTIFY authenticatingChanged)
    Q_PROPERTY(int minimumCodeLength READ minimumCodeLength CONSTANT)
    Q_PROPERTY(int maximumCodeLength READ maximumCodeLength CONSTANT)
    Q_PROPERTY(int maximumAttempts READ maximumAttempts NOTIFY maximumAttemptsChanged)
    Q_PROPERTY(bool codeInputIsKeyboard READ codeInputIsKeyboard NOTIFY codeInputIsKeyboardChanged)
    Q_ENUMS(Feedback)
    Q_ENUMS(Error)
    Q_ENUMS(Method)
    Q_FLAGS(Methods)
public:
    enum Feedback {
        PartialPrint,
        PrintIsUnclear,
        SensorIsDirty,
        SwipeFaster,
        SwipeSlower,
        UnrecognizedFinger,
        IncorrectLockCode
    };

    enum Error {
        LockedOut,
        SoftwareError,
        Canceled
    };

    enum Method {
        NoAuthentication    = 0x00,
        LockCode            = 0x01,
        Fingerprint         = 0x02
    };

    Q_DECLARE_FLAGS(Methods, Method)

    explicit Authenticator(QObject *parent = nullptr);
    ~Authenticator();

    Methods availableMethods() const;
    Methods utilizedMethods() const;
    bool isAuthenticating() const;

    int minimumCodeLength() const;
    int maximumCodeLength() const;
    int maximumAttempts() const;
    bool codeInputIsKeyboard() const;

    Q_INVOKABLE void authenticate(
            const QVariant &challengeCode, Methods methods = Methods(LockCode | Fingerprint));
    Q_INVOKABLE void enterLockCode(const QString &code);
    Q_INVOKABLE void cancel();

signals:
    void availableMethodsChanged();
    void utilizedMethodsChanged();
    void authenticatingChanged();
    void codeInputIsKeyboardChanged();
    void maximumAttemptsChanged();

    void authenticated(const QVariant &authenticationToken);
    void feedback(Feedback feedback, int attemptsRemaining);
    void error(Error error);

private:
    friend class AuthenticatorAdaptor;

    void connected();
    void disconnected();

    void handleAuthentication(const QVariant &authenticationToken);
    void handleFeedback(Feedback feedback, int attemptsRemaining, Methods utilizedMethods);
    void handleError(Error error);

    AuthenticatorAdaptor m_adaptor;
    QExplicitlySharedDataPointer<SettingsWatcher> m_settings;
    Methods m_availableMethods;
    Methods m_utilizedMethods;
    bool m_authenticating;
};

}

Q_DECLARE_OPERATORS_FOR_FLAGS(NemoDeviceLock::Authenticator::Methods)

#endif
