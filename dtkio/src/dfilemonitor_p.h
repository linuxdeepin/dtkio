// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFILEMONITOR_P_H
#define DFILEMONITOR_P_H

#include <QUrl>

#include "dtkio_global.h"

DIO_BEGIN_NAMESPACE
class DFileMonitor;
class DFileMonitorPrivate
{
public:
    explicit DFileMonitorPrivate(DFileMonitor *q);
    ~DFileMonitorPrivate();

    DFileMonitor *q = nullptr;
};
DIO_END_NAMESPACE

#endif   // DFILEMONITOR_P_H
