// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DDEVICEMANAGER_H
#define DDEVICEMANAGER_H

#include "namespace.h"

#include <QObject>

DMOUNT_BEGIN_NAMESPACE

class DDeviceManager : public QObject
{
    Q_OBJECT

public:
    explicit DDeviceManager(QObject *parent = nullptr);

    static QStringList blockDevices();
};

DMOUNT_END_NAMESPACE

#endif   // DDEVICEMANAGER_H
