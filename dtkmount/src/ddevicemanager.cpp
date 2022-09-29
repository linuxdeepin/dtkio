// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "ddevicemanager.h"

DMOUNT_BEGIN_NAMESPACE

DDeviceManager::DDeviceManager(QObject *parent)
    : QObject(parent)
{
}

QStringList DDeviceManager::blockDevices()
{
    // TODO(zhans): impl me!
    return {};
}

DMOUNT_END_NAMESPACE
