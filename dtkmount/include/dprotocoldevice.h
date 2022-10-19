// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DPROTOCOLDEVICE_H
#define DPROTOCOLDEVICE_H

#include <functional>

#include <QObject>
#include <QVariantMap>

#include "dtkmount_global.h"

DMOUNT_BEGIN_NAMESPACE

enum PasswdSaveMode {
    Never = 0,
    SaveInSession = 1,
    SaveForever = 2
};

namespace MountPasswdInfoParamKeys {
inline constexpr char kName[] { "name" };
inline constexpr char kPasswd[] { "passwd" };
inline constexpr char kDomain[] { "domain" };
inline constexpr char kAnonymous[] { "anonymous" };
inline constexpr char kCancel[] { "cancel" };
inline constexpr char kSaveMode[] { "mode" };
}   // namespace MountPasswdInfoParamKeys

// TODO(xust): error handle in callback
using OperateCallback = std::function<void(bool)>;
using OperateCallbackWithInfo = std::function<void(bool, const QString &)>;

using AskForPasswd = std::function<QVariantMap(const QString &msg, const QString &user, const QString &domain)>;
using AskForChoice = std::function<int(const QString &msg, const QStringList &choices)>;

class DProtocolDevicePrivate;
class DProtocolDevice : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DProtocolDevice)

public:
    explicit DProtocolDevice(const QString &devicePath, QObject *parent = nullptr);
    ~DProtocolDevice() override;

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
