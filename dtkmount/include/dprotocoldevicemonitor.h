// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DPROTOCOLDEVICEMONITOR_H
#define DPROTOCOLDEVICEMONITOR_H

#include <QObject>

#include "namespace.h"

DMOUNT_BEGIN_NAMESPACE

enum class ProtocolDeviceProperty;
class DProtocolDeviceMonitorPrivate;

class DProtocolDeviceMonitor : public QObject
{
    Q_OBJECT
public:
    explicit DProtocolDeviceMonitor(QObject *parent = nullptr);
    virtual ~DProtocolDeviceMonitor();

    void setWatchChanges(bool watchChanges);
    bool watchChanges() const;
    QStringList devices() const;

Q_SIGNALS:
    // TODO(xust) should the property separated from Block devices?
    void propertyChanged(const QString &devUrl, const QMap<Dtk::Mount::ProtocolDeviceProperty, QVariant> &changes);
    void deviceAdded(const QString &devUrl);
    void deviceRemoved(const QString &devUrl);
    void mountAdded(const QString &devUrl, const QString &mountPoint);
    void mountRemoved(const QString &devUrl);

private:
    QScopedPointer<DProtocolDeviceMonitorPrivate> d_ptr;
    Q_DECLARE_PRIVATE(DProtocolDeviceMonitor)
};

DMOUNT_END_NAMESPACE

#endif   // DPROTOCOLDEVICEMONITOR_H
