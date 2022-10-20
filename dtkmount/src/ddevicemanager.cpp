// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "ddevicemanager.h"

#include "objectmanager_interface.h"
#include "udisks2_interface.h"
#include "dbus/udisks2_dbus_common.h"
#include "dblockdevice.h"
#include "ddiskdrive.h"
#include "dblockpartition.h"

DMOUNT_BEGIN_NAMESPACE

static QStringList getDBusNodeNameList(const QString &service, const QString &path, const QDBusConnection &connection)
{
    QDBusInterface ud2(service, path, "org.freedesktop.DBus.Introspectable", connection);
    const QDBusReply<QString> &reply = ud2.call("Introspect");
    QXmlStreamReader xmlParser(reply.value());
    QStringList nodeList;

    while (!xmlParser.atEnd()) {
        xmlParser.readNext();

        if (xmlParser.tokenType() == QXmlStreamReader::StartElement
            && xmlParser.name().toString() == "node") {
            const QString &name = xmlParser.attributes().value("name").toString();

            if (!name.isEmpty())
                nodeList << path + "/" + name;
        }
    }

    return nodeList;
}

DBlockDeviceMonitor *DDeviceManager::globalBlockDeviceMonitor()
{
    // TODO(zhangs): impl me
    return {};
}

DProtocolDeviceMonitor *DDeviceManager::globalProtocolDeviceMonitor()
{
    // TODO(xust): impl me
    return {};
}

QStringList DDeviceManager::blockDevices(const QVariantMap &options)
{
    OrgFreedesktopUDisks2ManagerInterface udisksmgr(kUDisks2Service, kUDisks2ManagerPath, QDBusConnection::systemBus());

    auto reply = udisksmgr.GetBlockDevices(options);
    reply.waitForFinished();
    const QList<QDBusObjectPath> &resultList = reply.value();
    QStringList dbusPaths;
    for (const QDBusObjectPath &singleResult : resultList)
        dbusPaths << singleResult.path();
    return dbusPaths;
}

QStringList DDeviceManager::protocolDevices()
{
    // TODO(xust): impl me
    return {};
}

QStringList DDeviceManager::diskDrives()
{
    return getDBusNodeNameList(kUDisks2Service, kUDisks2DrivePath, QDBusConnection::systemBus());
}

DBlockDevice *DDeviceManager::createBlockDevice(const QString &path, QObject *parent)
{
    return new DBlockDevice(path, parent);
}

DBlockDevice *DDeviceManager::createBlockDeviceByDevicePath(const QByteArray &path, QObject *parent)
{
    for (const QString &block : blockDevices()) {
        DBlockDevice *device = new DBlockDevice(block, parent);

        if (device->device() == path)
            return device;

        device->deleteLater();
    }

    return nullptr;
}

DBlockPartition *DDeviceManager::createBlockPartition(const QString &path, QObject *parent)
{
    return new DBlockPartition(path, parent);
}

DBlockPartition *DDeviceManager::createBlockPartitionByMountPoint(const QByteArray &path, QObject *parent)
{
    for (const QString &block : blockDevices()) {
        DBlockPartition *device = new DBlockPartition(block, parent);

        if (device->mountPoints().contains(path))
            return device;

        device->deleteLater();
    }

    return nullptr;
}

DDiskDrive *DDeviceManager::createDiskDrive(const QString &path, QObject *parent)
{
    return new DDiskDrive(path, parent);
}

DDiskJob *DDeviceManager::createDiskJob(const QString &path, QObject *parent)
{
    // TODO(zhangs): impl me
    return {};
}

DProtocolDevice *DDeviceManager::createProtocolDevice(const QString &path, QObject *parent)
{
    // TODO(xust): impl me
    return {};
}

QStringList DDeviceManager::supportedFilesystems()
{
    OrgFreedesktopUDisks2ManagerInterface udisksmgr(kUDisks2Service, kUDisks2ManagerPath, QDBusConnection::systemBus());
    return udisksmgr.supportedFilesystems();
}

QStringList DDeviceManager::supportedEncryptionTypes()
{
    OrgFreedesktopUDisks2ManagerInterface udisksmgr(kUDisks2Service, kUDisks2ManagerPath, QDBusConnection::systemBus());
    return udisksmgr.supportedEncryptionTypes();
}

QStringList DDeviceManager::resolveDevice(QVariantMap devspec, QVariantMap options)
{
    OrgFreedesktopUDisks2ManagerInterface udisksmgr(kUDisks2Service, kUDisks2ManagerPath, QDBusConnection::systemBus());
    QStringList ret;
    auto devices = udisksmgr.ResolveDevice(devspec, options);
    devices.waitForFinished();
    if (!devices.isError()) {
        for (const auto &d : devices.value())
            ret.push_back(d.path());
    }
    return ret;
}

QStringList DDeviceManager::resolveDeviceNode(QString devnode, QVariantMap options)
{
    return resolveDevice({ { "path", QVariant(devnode) } }, options);
}

bool DDeviceManager::canCheck(const QString &type, QString *requiredUtil)
{
    OrgFreedesktopUDisks2ManagerInterface udisksmgr(kUDisks2Service, kUDisks2ManagerPath, QDBusConnection::systemBus());
    auto r = udisksmgr.CanCheck(type);
    r.waitForFinished();
    if (r.isError())
        return false;

    if (requiredUtil)
        *requiredUtil = r.value().second;

    return r.value().first;
}

bool DDeviceManager::canFormat(const QString &type, QString *requiredUtil)
{
    OrgFreedesktopUDisks2ManagerInterface udisksmgr(kUDisks2Service, kUDisks2ManagerPath, QDBusConnection::systemBus());
    auto r = udisksmgr.CanFormat(type);
    r.waitForFinished();
    if (r.isError())
        return false;

    if (requiredUtil)
        *requiredUtil = r.value().second;

    return r.value().first;
}

bool DDeviceManager::canRepair(const QString &type, QString *requiredUtil)
{
    OrgFreedesktopUDisks2ManagerInterface udisksmgr(kUDisks2Service, kUDisks2ManagerPath, QDBusConnection::systemBus());
    auto r = udisksmgr.CanRepair(type);
    r.waitForFinished();
    if (r.isError())
        return false;

    if (requiredUtil)
        *requiredUtil = r.value().second;

    return r.value().first;
}

bool DDeviceManager::canResize(const QString &type, QString *requiredUtil)
{
    OrgFreedesktopUDisks2ManagerInterface udisksmgr(kUDisks2Service, kUDisks2ManagerPath, QDBusConnection::systemBus());
    auto r = udisksmgr.CanResize(type);
    r.waitForFinished();
    if (r.isError())
        return false;

    if (requiredUtil)
        *requiredUtil = r.value().second.second;

    return r.value().first;
}

QString DDeviceManager::loopSetup(int fd, QVariantMap options)
{
    OrgFreedesktopUDisks2ManagerInterface udisksmgr(kUDisks2Service, kUDisks2ManagerPath, QDBusConnection::systemBus());
    QDBusUnixFileDescriptor dbusfd;
    dbusfd.setFileDescriptor(fd);
    auto r = udisksmgr.LoopSetup(dbusfd, options);
    r.waitForFinished();
    return r.value().path();
}

DMOUNT_END_NAMESPACE
