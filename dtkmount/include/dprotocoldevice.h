// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DPROTOCOLDEVICE_H
#define DPROTOCOLDEVICE_H

#include <functional>

#include <QObject>
#include <QVariantMap>

#include "namespace.h"

DMOUNT_BEGIN_NAMESPACE

enum PasswdSave {
    kNever = 0,
    kSaveInSession,
    kSaveForever
};

struct MountPasswdInfo
{
    QString name;
    QString passwd;
    QString domain;
    bool anonymous { false };
    bool cancel { false };
    PasswdSave mode { kNever };
};

// TODO(xust): error handle in callback
using OperateCallback = std::function<void(bool)>;
using OperateCallbackWithInfo = std::function<void(bool, const QString &)>;

using AskForPasswd = std::function<MountPasswdInfo(const QString &msg, const QString &user, const QString &domain)>;
using AskForChoice = std::function<int(const QString &msg, const QStringList &choices)>;

class DProtocolDevicePrivate;
class DProtocolDevice : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DProtocolDevice)

public:
    explicit DProtocolDevice(const QString &devicePath, QObject *parent = nullptr);
    virtual ~DProtocolDevice();

    QString path() const;
    QString mountPoint() const;
    QString displayName() const;
    QString fileSystem() const;
    quint64 sizeTotal() const;
    quint64 sizeFree() const;
    quint64 sizeUsage() const;
    QStringList iconNames() const;

    QString mount(const QVariantMap &opts = {});
    void mountAsync(const QVariantMap &opts = {}, OperateCallbackWithInfo callback = nullptr);
    bool unmount(const QVariantMap &opts = {});
    void unmountAsync(const QVariantMap &opts = {}, OperateCallback callback = nullptr);

    void setOperateTimeout(int secs = 3);
    void setAskForPasswd(AskForPasswd callback);
    void setAskForChoice(AskForChoice callback);

private:
    QScopedPointer<DProtocolDevicePrivate> d_ptr;
};

DMOUNT_END_NAMESPACE

#endif   // DPROTOCOLDEVICE_H
