// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dfilewatcher_p.h"

#include <DFileWatcher>

DIO_BEGIN_NAMESPACE

DFileWatcherPrivate::DFileWatcherPrivate(DFileWatcher *q)
{
}

DFileWatcherPrivate::~DFileWatcherPrivate()
{
}

DFileWatcher::DFileWatcher(const QUrl &url, QObject *parent)
    : QObject(parent)
{
}

DFileWatcher::~DFileWatcher()
{
}

QUrl DFileWatcher::url() const
{
    return QUrl();
}

void DFileWatcher::setTimeRate(quint32 msec)
{
}

quint32 DFileWatcher::timeRate() const
{
    return quint32();
}

void DFileWatcher::setWatchType(WatchType type)
{
}

WatchType DFileWatcher::watchType() const
{
    return WatchType::Auto;
}

bool DFileWatcher::running() const
{
    return false;
}

bool DFileWatcher::start()
{
    return false;
}

bool DFileWatcher::start(quint32 timeRate)
{
    return false;
}

DFileFuture *DFileWatcher::startAsync(int ioPriority, QObject *parent)
{
    return nullptr;
}

bool DFileWatcher::stop()
{
    return false;
}

DIO_END_NAMESPACE
