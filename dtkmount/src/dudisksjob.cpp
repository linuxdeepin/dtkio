// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dudisksjob.h"
#include "dudisksjob_p.h"

DMOUNT_USE_NAMESPACE

DUDisksJob::DUDisksJob(QObject *parent)
    : QObject { parent }, d_ptr { new DUDisksJobPrivate(this) }
{
}

DUDisksJob::~DUDisksJob()
{
}

QString DUDisksJob::path() const
{
    Q_D(const DUDisksJob);
    return d->iface->path();
}

QStringList DUDisksJob::objects() const
{
    Q_D(const DUDisksJob);
    QStringList ret;
    auto &&objs = d->iface->objects();
    for (const auto &obj : objs)
        ret << obj.path();
    return ret;
}

bool DUDisksJob::cancelable() const
{
    Q_D(const DUDisksJob);
    return d->iface->cancelable();
}

bool DUDisksJob::progressValid() const
{
    Q_D(const DUDisksJob);
    return d->iface->progressValid();
}

double DUDisksJob::progress() const
{
    Q_D(const DUDisksJob);
    return d->iface->progress();
}

QString DUDisksJob::operation() const
{
    Q_D(const DUDisksJob);
    return d->iface->operation();
}

quint32 DUDisksJob::startedByUid() const
{
    Q_D(const DUDisksJob);
    return d->iface->startedByUID();
}

quint64 DUDisksJob::bytes() const
{
    Q_D(const DUDisksJob);
    return d->iface->bytes();
}

quint64 DUDisksJob::expectedEndTime() const
{
    Q_D(const DUDisksJob);
    return d->iface->expectedEndTime();
}

quint64 DUDisksJob::rate() const
{
    Q_D(const DUDisksJob);
    return d->iface->rate();
}

quint64 DUDisksJob::startTime() const
{
    Q_D(const DUDisksJob);
    return d->iface->startTime();
}

void DUDisksJob::cancel(const QVariantMap &options)
{
    Q_D(DUDisksJob);
    d->iface->Cancel(options).waitForFinished();
}
