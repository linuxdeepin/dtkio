// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dfilemonitor_p.h"
#include "dfilemonitor.h"

DIO_BEGIN_NAMESPACE

DFileMonitorPrivate::DFileMonitorPrivate(DFileMonitor *q)
{
}

DFileMonitorPrivate::~DFileMonitorPrivate()
{
}

DFileMonitor::DFileMonitor(const QUrl &url, QObject *parent)
    : QObject(parent)
{
}

DFileMonitor::~DFileMonitor()
{
}

QUrl DFileMonitor::url() const
{
    return QUrl();
}

void DFileMonitor::setTimeRate(quint32 msec)
{
}

quint32 DFileMonitor::timeRate() const
{
    return quint32();
}

void DFileMonitor::setWatchType(WatchType type)
{
}

WatchType DFileMonitor::watchType() const
{
    return WatchType::Auto;
}

bool DFileMonitor::running() const
{
    return false;
}

bool DFileMonitor::start()
{
    return false;
}

bool DFileMonitor::start(quint32 timeRate)
{
    return false;
}

DFileFuture *DFileMonitor::startAsync(int ioPriority, QObject *parent)
{
    return nullptr;
}

bool DFileMonitor::stop()
{
    return false;
}

DIO_END_NAMESPACE
