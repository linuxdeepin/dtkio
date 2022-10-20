// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DDEVICEMANAGER_H
#define DDEVICEMANAGER_H

#include <QObject>
#include <QVariantMap>

#include "dtkmount_global.h"

DMOUNT_BEGIN_NAMESPACE

class DBlockDeviceMonitor;
class DProtocolDeviceMonitor;
class DBlockDevice;
class DBlockPartition;
class DDiskDrive;
class DDiskJob;
class DProtocolDevice;

namespace DDeviceManager {
DBlockDeviceMonitor *globalBlockDeviceMonitor();
DProtocolDeviceMonitor *globalProtocolDeviceMonitor();
QStringList blockDevices(const QVariantMap &options = {});
QStringList protocolDevices();
QStringList diskDrives();

[[nodiscard]] DBlockDevice *createBlockDevice(const QString &path, QObject *parent = nullptr);
[[nodiscard]] DBlockDevice *createBlockDeviceByDevicePath(const QByteArray &path, QObject *parent = nullptr);
[[nodiscard]] DBlockPartition *createBlockPartition(const QString &path, QObject *parent = nullptr);
[[nodiscard]] DBlockPartition *createBlockPartitionByMountPoint(const QByteArray &path, QObject *parent = nullptr);
[[nodiscard]] DDiskDrive *createDiskDrive(const QString &path, QObject *parent = nullptr);
[[nodiscard]] DDiskJob *createDiskJob(const QString &path, QObject *parent = nullptr);
[[nodiscard]] DProtocolDevice *createProtocolDevice(const QString &path, QObject *parent = nullptr);

QStringList supportedFilesystems();
QStringList supportedEncryptionTypes();
QStringList resolveDevice(QVariantMap devspec, QVariantMap options);
QStringList resolveDeviceNode(QString devnode, QVariantMap options);
bool canCheck(const QString &type, QString *requiredUtil = nullptr);
bool canFormat(const QString &type, QString *requiredUtil = nullptr);
bool canRepair(const QString &type, QString *requiredUtil = nullptr);
bool canResize(const QString &type, QString *requiredUtil = nullptr);
QString loopSetup(int fd, QVariantMap options);

// TODO(zhangs): error handle

}   // namespace DDeviceManager

DMOUNT_END_NAMESPACE

#endif   // DDEVICEMANAGER_H
