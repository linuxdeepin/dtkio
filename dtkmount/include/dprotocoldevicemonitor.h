// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DPROTOCOLDEVICEMONITOR_H
#define DPROTOCOLDEVICEMONITOR_H

#include <QObject>

#include "dtkmount_global.h"

DMOUNT_BEGIN_NAMESPACE

class DProtocolDeviceMonitorPrivate;
class DProtocolDeviceMonitor : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DProtocolDeviceMonitor)

public:
    ~DProtocolDeviceMonitor() override;

    void setWatchChanges(bool watchChanges);
    bool watchChanges() const;
    QStringList devices() const;

Q_SIGNALS:
    void deviceAdded(const QString &devUrl);
    void deviceRemoved(const QString &devUrl);
    void mountAdded(const QString &devUrl, const QString &mountPoint);
    void mountRemoved(const QString &devUrl);

private:
    explicit DProtocolDeviceMonitor(QObject *parent = nullptr);

    QScopedPointer<DProtocolDeviceMonitorPrivate> d_ptr;
};

DMOUNT_END_NAMESPACE

#endif   // DPROTOCOLDEVICEMONITOR_H
