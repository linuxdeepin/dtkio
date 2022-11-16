// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "denumerator_p.h"
#include "denumerator.h"

DIO_BEGIN_NAMESPACE

DEnumeratorPrivate::DEnumeratorPrivate(DEnumerator *q)
{
}

DEnumeratorPrivate::~DEnumeratorPrivate()
{
}

DEnumerator::DEnumerator(const QUrl &url)
{
}

DEnumerator::~DEnumerator()
{
}

QUrl DEnumerator::url() const
{
    return QUrl();
}

void DEnumerator::setNameFilters(const QStringList &nameFilters)
{
}

void DEnumerator::setDirFilters(DirFilters dirFilters)
{
}

void DEnumerator::setIteratorFlags(IteratorFlags flags)
{
}

QStringList DEnumerator::nameFilters() const
{
    return QStringList();
}

DirFilters DEnumerator::dirFilters() const
{
    return DirFilter::NoFilter;
}

IteratorFlags DEnumerator::iteratorFlags() const
{
    return IteratorFlag::NoIteratorFlags;
}

void DEnumerator::setTimeout(quint64 timeout)
{
}

quint64 DEnumerator::timeout() const
{
    return quint64();
}

bool DEnumerator::createEnumerator()
{
    return false;
}

DFileFuture *DEnumerator::createEnumeratorAsync(int ioPriority, QObject *parent)
{
    return nullptr;
}

bool DEnumerator::hasNext() const
{
    return false;
}

QUrl DEnumerator::next() const
{
    return QUrl();
}

DFileInfo *DEnumerator::createFileInfo(QObject *parent) const
{
    return nullptr;
}

quint64 DEnumerator::fileCount()
{
    return quint64();
}

DIO_END_NAMESPACE
