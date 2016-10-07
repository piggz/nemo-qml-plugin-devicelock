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

#ifndef DBUSUTILITIES_H
#define DBUSUTILITIES_H

#include <QDateTime>
#include <QDBusArgument>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusVariant>
#include <QVector>

struct Fingerprint
{
    QVariant id;
    QString name;
    QDateTime acquisitionDate;
};

Q_DECLARE_METATYPE(Fingerprint)
Q_DECLARE_METATYPE(QVector<Fingerprint>)

QDBusArgument &operator<<(QDBusArgument &argument, const Fingerprint &fingerprint);
const QDBusArgument &operator>>(const QDBusArgument &argument, Fingerprint &fingerprint);

template <typename Argument> inline QVariant marshallArgument(const Argument &argument) {
    return QVariant::fromValue(argument); }
inline QVariant marshallArgument(const QVariant &argument) {
    return QVariant::fromValue(QDBusVariant(argument)); }

template <typename Argument> inline Argument demarshallArgument(const QVariant &argument) {
    return argument.value<Argument>(); }
template <> inline QVariant demarshallArgument<QVariant>(const QVariant &argument) {
    return argument.value<QDBusVariant>().variant(); }

inline void appendArguments(QVariantList &) {}

template<typename Argument, typename... Arguments>
inline void appendArguments(QVariantList &list, Argument value, Arguments... arguments)
{
    list.append(marshallArgument(value));
    appendArguments(list, arguments...);
}

template <typename... Arguments> inline QVariantList marshallArguments(Arguments... arguments)
{
    QVariantList list;
    appendArguments(list, arguments...);
    return list;
}

template <typename... Arguments> inline bool send(
            const QDBusConnection &connection,
            const QString &path,
            const QString &interface,
            const QString &method,
            Arguments... arguments)
{
    QDBusMessage message = QDBusMessage::createMethodCall(QString(), path, interface, method);
    message.setArguments(marshallArguments(arguments...));
    return connection.send(message);
}

template <typename... Arguments> inline bool send(
            const QString &connectionName,
            const QString &path,
            const QString &interface,
            const QString &method,
            Arguments... arguments)
{
    return send(QDBusConnection(connectionName), path, interface, method, arguments...);
}

bool operator ==(const QDBusConnection &left, const QDBusConnection &right);
bool operator !=(const QDBusConnection &left, const QDBusConnection &right);

#endif
