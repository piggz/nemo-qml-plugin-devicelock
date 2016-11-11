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

#ifndef NEMODEVICELOCK_AUTHENTICATIONINPUT_H
#define NEMODEVICELOCK_AUTHENTICATIONINPUT_H

#include <nemo-devicelock/authenticator.h>

namespace NemoDeviceLock {

class AuthenticationInput;
class AuthenticationInputAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.nemomobile.devicelock.client.AuthenticationInput")
public:
    explicit AuthenticationInputAdaptor(AuthenticationInput *authenticationInput);

public slots:
    Q_NOREPLY void AuthenticationStarted(uint pid, uint utilizedMethods, uint instruction);
    Q_NOREPLY void AuthenticationUnavailable(uint pid, uint error);
    Q_NOREPLY void AuthenticationEvaluating();
    Q_NOREPLY void AuthenticationEnded(bool confirmed);
    Q_NOREPLY void Feedback(uint feedback, uint attemptsRemaining, uint utilizedMethods);
    Q_NOREPLY void Error(uint error);

private:
    AuthenticationInput *m_authenticationInput;
};

class SettingsWatcher;
class NEMODEVICELOCK_EXPORT AuthenticationInput : public QObject, private ConnectionClient
{
    Q_OBJECT
    Q_PROPERTY(Authenticator::Methods utilizedMethods READ utilizedMethods NOTIFY utilizedMethodsChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(bool authenticatingProcess READ authenticatingPid NOTIFY authenticatingPidChanged)
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(bool registered READ isRegistered WRITE setRegistered NOTIFY registeredChanged)
    Q_PROPERTY(int minimumCodeLength READ minimumCodeLength CONSTANT)
    Q_PROPERTY(int maximumCodeLength READ maximumCodeLength CONSTANT)
    Q_PROPERTY(int maximumAttempts READ maximumAttempts NOTIFY maximumAttemptsChanged)
    Q_PROPERTY(bool codeInputIsKeyboard READ codeInputIsKeyboard NOTIFY codeInputIsKeyboardChanged)
    Q_ENUMS(Feedback)
    Q_ENUMS(Error)
    Q_ENUMS(Status)
public:
    enum Feedback {
        EnterSecurityCode,
        EnterNewSecurityCode,
        RepeatNewSecurityCode,
        SecurityCodesDoNotMatch,
        SecurityCodeInHistory,
        SecurityCodeExpired,
        PartialPrint,
        PrintIsUnclear,
        SensorIsDirty,
        SwipeFaster,
        SwipeSlower,
        UnrecognizedFinger,
        IncorrectSecurityCode
    };

    enum Error {
        LockedOut,
        Canceled,
        SoftwareError
    };

    enum Type {
        Authentication,
        DeviceLock
    };

    enum Status {
        Idle,
        Authenticating,
        Evaluating,
        AuthenticationError
    };

    explicit AuthenticationInput(Type type = Authentication, QObject *parent = nullptr);
    ~AuthenticationInput();

    Status status() const;
    int authenticatingPid() const;
    Authenticator::Methods utilizedMethods() const;

    bool isActive() const;
    void setActive(bool active);

    bool isRegistered() const;
    void setRegistered(bool registered);

    int maximumAttempts() const;

    int minimumCodeLength() const;
    int maximumCodeLength() const;
    bool codeInputIsKeyboard() const;

    Q_INVOKABLE void enterSecurityCode(const QString &code);
    Q_INVOKABLE void cancel();

signals:
    void activeChanged();
    void registeredChanged();
    void statusChanged();
    void authenticatingPidChanged();
    void utilizedMethodsChanged();
    void maximumAttemptsChanged();
    void codeInputIsKeyboardChanged();

    void authenticationStarted(Feedback feedback);
    void authenticationUnavailable(Error error);
    void authenticationEvaluating();
    void authenticationEnded(bool confirmed);

    void feedback(Feedback feedback, int attemptsRemaining);
    void error(Error error);

private:
    friend class AuthenticationInputAdaptor;

    inline void connected();

    inline void handleAuthenticationStarted(
            int pid, Authenticator::Methods utilizedMethods, Feedback feedback);
    inline void handleAuthenticationUnavailable(int pid, Error error);
    inline void handleAuthenticationEvaluating();
    inline void handleAuthenticationEnded(bool confirmed);
    inline void handleFeedback(
            Feedback feedback, int attemptsRemaining, Authenticator::Methods utilizedMethods);
    inline void handleError(Error error);

    AuthenticationInputAdaptor m_adaptor;
    QExplicitlySharedDataPointer<SettingsWatcher> m_settings;
    Authenticator::Methods m_utilizedMethods;
    int m_authenticatingPid;
    Status m_status;
    bool m_registered;
    bool m_active;
};

}

#endif