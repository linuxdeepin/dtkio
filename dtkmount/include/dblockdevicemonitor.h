// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DBLOCKDEVICEMONITOR_H
#define DBLOCKDEVICEMONITOR_H

#include <QObject>

#include "dtkmount_global.h"

DMOUNT_BEGIN_NAMESPACE

enum class BlockDeviceProperty;
class DBlockDeviceMonitorPrivate;
class DBlockDeviceMonitor : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DBlockDeviceMonitor);

public:
    explicit DBlockDeviceMonitor(QObject *parent = nullptr);
    virtual ~DBlockDeviceMonitor();

    void setWatchChanges(bool watchChanges);
    bool watchChanges() const;

Q_SIGNALS:
    void deviceAdded(const QString &devPath);
    void deviceRemoved(const QString &devPath);
    void mountAdded(const QString &devPath, const QString &mountPoint);
    void mountRemoved(const QString &devPath);
    void fileSystemAdded(const QString &devPath);
    void fileSystemRemoved(const QString &devPath);
    void driveAdded(const QString &drvPath);
    void driveRemoved(const QString &drvPath);
    void propertiesChanged(const QString &devPath, const QMap<Dtk::Mount::BlockDeviceProperty, QVariant> &changes);

private:
    QScopedPointer<DBlockDeviceMonitorPrivate> d_ptr;
};

DMOUNT_END_NAMESPACE

#endif   // DBLOCKDEVICEMONITOR_H
