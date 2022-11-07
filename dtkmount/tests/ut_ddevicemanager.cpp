// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "ddevicemanager.h"

#include <QDBusPendingReply>

#include <gtest/gtest.h>
#include <stubext.h>

#include "udisks2_interface.h"

DMOUNT_BEGIN_NAMESPACE

class TestDDeviceManager : public testing::Test
{
public:
    void SetUp() override { }
    void TearDown() override { m_stub.clear(); }

    stub_ext::StubExt m_stub;
};

TEST_F(TestDDeviceManager, globalBlockDeviceMonitor)
{
    EXPECT_NO_FATAL_FAILURE(DDeviceManager::globalBlockDeviceMonitor());
}

TEST_F(TestDDeviceManager, globalProtocolDeviceMonitor)
{
    EXPECT_NO_FATAL_FAILURE(DDeviceManager::globalProtocolDeviceMonitor());
}

TEST_F(TestDDeviceManager, blockDevices)
{
    m_stub.set_lamda(&OrgFreedesktopUDisks2ManagerInterface::GetBlockDevices, [] {
        __DBG_STUB_INVOKE__
        return QDBusPendingReply<QList<QDBusObjectPath>>();
    });
    m_stub.set_lamda(&QDBusPendingReply<QList<QDBusObjectPath>>::waitForFinished,
                     []() { __DBG_STUB_INVOKE__ });
    m_stub.set_lamda(&QDBusPendingReply<QList<QDBusObjectPath>>::value,
                     []() {
                         __DBG_STUB_INVOKE__
                         return QList<QDBusObjectPath>() << QDBusObjectPath("/test");
                     });
    auto &&devices = DDeviceManager::blockDevices();
    //    EXPECT_EQ(devices.value().size(), 1); // FIXME(xust):
    //    EXPECT_TRUE(devices.value().at(0) == "/test");
}

TEST_F(TestDDeviceManager, protocolDevices)
{
    // TODO(zhangs): imple me!
}

TEST_F(TestDDeviceManager, diskDrives)
{
    QString testXml { "<node>"
                      "<node name=\"Manager\"/>"
                      "<node name=\"drives\"/>"
                      "<node name=\"block_devices\"/>"
                      "</node>" };
    m_stub.set_lamda(&QDBusReply<QString>::value, [=]() { __DBG_STUB_INVOKE__ return testXml; });
    auto &&drives = DDeviceManager::diskDrives();
    EXPECT_EQ(drives.size(), 3);
    EXPECT_TRUE(drives.at(0).endsWith("Manager"));
    EXPECT_TRUE(drives.at(1).endsWith("drives"));
    EXPECT_TRUE(drives.at(2).endsWith("block_devices"));
}

TEST_F(TestDDeviceManager, createBlockDevice)
{
    // TODO(zhangs): imple me!
}

TEST_F(TestDDeviceManager, createBlockDeviceByDevicePath)
{
    // TODO(zhangs): imple me!
}

TEST_F(TestDDeviceManager, createBlockPartition)
{
    // TODO(zhangs): imple me!
}

TEST_F(TestDDeviceManager, createBlockPartitionByMountPoint)
{
    // TODO(zhangs): imple me!
}

TEST_F(TestDDeviceManager, createDiskDrive)
{
    // TODO(zhangs): imple me!
}

TEST_F(TestDDeviceManager, createDiskJob)
{
    // TODO(zhangs): imple me!
}

TEST_F(TestDDeviceManager, createProtocolDevice)
{
    // TODO(zhangs): imple me!
}

TEST_F(TestDDeviceManager, supportedFilesystems)
{
    m_stub.set_lamda(&OrgFreedesktopUDisks2ManagerInterface::supportedFilesystems,
                     []() {
        __DBG_STUB_INVOKE__
        return  QStringList() << "ext4"; });
    auto &&fs = DDeviceManager::supportedFilesystems();
    EXPECT_EQ(fs.size(), 1);
    EXPECT_EQ(fs.at(0), "ext4");
}

TEST_F(TestDDeviceManager, supportedEncryptionTypes)
{
    m_stub.set_lamda(&OrgFreedesktopUDisks2ManagerInterface::supportedEncryptionTypes,
                     []() {
        __DBG_STUB_INVOKE__
        return  QStringList(); });
    auto &&types = DDeviceManager::supportedEncryptionTypes();
    EXPECT_TRUE(types.isEmpty());
}

TEST_F(TestDDeviceManager, resolveDevice)
{
    m_stub.set_lamda(&OrgFreedesktopUDisks2ManagerInterface::ResolveDevice, [] {
        __DBG_STUB_INVOKE__
        return QDBusPendingReply<QList<QDBusObjectPath>>();
    });
    m_stub.set_lamda(&QDBusPendingReply<QList<QDBusObjectPath>>::waitForFinished,
                     []() { __DBG_STUB_INVOKE__ });
    m_stub.set_lamda(&QDBusPendingCall::isError, []() { __DBG_STUB_INVOKE__ return false; });
    m_stub.set_lamda(&QDBusPendingReply<QList<QDBusObjectPath>>::value,
                     []() {
                         __DBG_STUB_INVOKE__
                         return QList<QDBusObjectPath>() << QDBusObjectPath("/test");
                     });

    auto devices = DDeviceManager::resolveDevice({}, {});
    //    EXPECT_EQ(devices.value().size(), 1); // FIXME(xust):
    //    EXPECT_TRUE(devices.value().at(0) == "/test");
}

TEST_F(TestDDeviceManager, canCheck)
{
    m_stub.set_lamda(&OrgFreedesktopUDisks2ManagerInterface::CanCheck, [] {
        __DBG_STUB_INVOKE__
        return QDBusPendingReply<QPair<bool, QString>>();
    });
    m_stub.set_lamda(&QDBusPendingReply<QPair<bool, QString>>::waitForFinished,
                     []() { __DBG_STUB_INVOKE__ });
    m_stub.set_lamda(&QDBusPendingReply<QPair<bool, QString>>::isError,
                     []() { __DBG_STUB_INVOKE__ return true; });
    EXPECT_FALSE(DDeviceManager::canCheck("", nullptr));

    m_stub.set_lamda(&QDBusPendingReply<QPair<bool, QString>>::isError,
                     []() { __DBG_STUB_INVOKE__ return false; });
    m_stub.set_lamda(&QDBusPendingReply<QPair<bool, QString>>::value,
                     []() {
                         __DBG_STUB_INVOKE__
                         return QPair<bool, QString>(true, "test");
                     });
    QString ret;
    EXPECT_TRUE(DDeviceManager::canCheck("", &ret));
    EXPECT_EQ(ret, "test");
}

TEST_F(TestDDeviceManager, canFormat)
{
    m_stub.set_lamda(&OrgFreedesktopUDisks2ManagerInterface::CanFormat, [] {
        __DBG_STUB_INVOKE__
        return QDBusPendingReply<QPair<bool, QString>>();
    });
    m_stub.set_lamda(&QDBusPendingReply<QPair<bool, QString>>::waitForFinished,
                     []() { __DBG_STUB_INVOKE__ });
    m_stub.set_lamda(&QDBusPendingReply<QPair<bool, QString>>::isError,
                     []() { __DBG_STUB_INVOKE__ return true; });
    EXPECT_FALSE(DDeviceManager::canFormat("", nullptr));

    m_stub.set_lamda(&QDBusPendingReply<QPair<bool, QString>>::isError,
                     []() { __DBG_STUB_INVOKE__ return false; });
    m_stub.set_lamda(&QDBusPendingReply<QPair<bool, QString>>::value,
                     []() {
                         __DBG_STUB_INVOKE__
                         return QPair<bool, QString>(true, "test");
                     });
    QString ret;
    EXPECT_TRUE(DDeviceManager::canFormat("", &ret));
    EXPECT_EQ(ret, "test");
}

TEST_F(TestDDeviceManager, canRepair)
{
    m_stub.set_lamda(&OrgFreedesktopUDisks2ManagerInterface::CanRepair, [] {
        __DBG_STUB_INVOKE__
        return QDBusPendingReply<QPair<bool, QString>>();
    });
    m_stub.set_lamda(&QDBusPendingReply<QPair<bool, QString>>::waitForFinished,
                     []() { __DBG_STUB_INVOKE__ });
    m_stub.set_lamda(&QDBusPendingReply<QPair<bool, QString>>::isError,
                     []() { __DBG_STUB_INVOKE__ return true; });
    EXPECT_FALSE(DDeviceManager::canRepair("", nullptr));

    m_stub.set_lamda(&QDBusPendingReply<QPair<bool, QString>>::isError,
                     []() { __DBG_STUB_INVOKE__ return false; });
    m_stub.set_lamda(&QDBusPendingReply<QPair<bool, QString>>::value,
                     []() {
                         __DBG_STUB_INVOKE__
                         return QPair<bool, QString>(true, "test");
                     });
    QString ret;
    EXPECT_TRUE(DDeviceManager::canRepair("", &ret));
    EXPECT_EQ(ret, "test");
}

TEST_F(TestDDeviceManager, canResize)
{
    m_stub.set_lamda(&OrgFreedesktopUDisks2ManagerInterface::CanResize, [] {
        __DBG_STUB_INVOKE__
        return QDBusPendingReply<QPair<bool, QPair<quint64, QString>>>();
    });
    m_stub.set_lamda(&QDBusPendingReply<QPair<bool, QPair<quint64, QString>>>::waitForFinished,
                     []() { __DBG_STUB_INVOKE__ });
    m_stub.set_lamda(&QDBusPendingReply<QPair<bool, QPair<quint64, QString>>>::isError,
                     []() { __DBG_STUB_INVOKE__ return true; });
    EXPECT_FALSE(DDeviceManager::canResize("", nullptr));

    m_stub.set_lamda(&QDBusPendingReply<QPair<bool, QPair<quint64, QString>>>::isError,
                     []() { __DBG_STUB_INVOKE__ return false; });
    m_stub.set_lamda(&QDBusPendingReply<QPair<bool, QPair<quint64, QString>>>::value,
                     []() {
                         __DBG_STUB_INVOKE__
                         return QPair<bool, QPair<quint64, QString>>(true, { 0, "test" });
                     });
    QString ret;
    EXPECT_TRUE(DDeviceManager::canResize("", &ret));
    EXPECT_EQ(ret, "test");
}

TEST_F(TestDDeviceManager, loopSetup)
{
    m_stub.set_lamda(&OrgFreedesktopUDisks2ManagerInterface::LoopSetup, [] {
        __DBG_STUB_INVOKE__
        return QDBusPendingReply<QDBusObjectPath>();
    });
    m_stub.set_lamda(&QDBusPendingReply<QDBusPendingReply<QDBusObjectPath>>::waitForFinished,
                     []() { __DBG_STUB_INVOKE__ });
    m_stub.set_lamda(&QDBusPendingReply<QDBusObjectPath>::value,
                     []() {
                         __DBG_STUB_INVOKE__
                         return QDBusObjectPath("/test");
                     });
    //    EXPECT_EQ(DDeviceManager::loopSetup(10, {}), "/test"); // FIXME(xust):
}

DMOUNT_END_NAMESPACE
