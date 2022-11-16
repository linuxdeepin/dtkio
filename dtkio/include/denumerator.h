// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DENUMERATOR_H
#define DENUMERATOR_H

#include <QUrl>
#include <QScopedPointer>

#include "dtkio_global.h"
#include "dtkiotypes.h"

DIO_BEGIN_NAMESPACE
class DFileFuture;
class DFileInfo;
class DEnumeratorPrivate;
class DEnumerator
{
public:
    explicit DEnumerator(const QUrl &url);
    ~DEnumerator();

    QUrl url() const;

    void setNameFilters(const QStringList &nameFilters);
    void setDirFilters(DirFilters dirFilters);
    void setIteratorFlags(IteratorFlags flags);
    QStringList nameFilters() const;
    DirFilters dirFilters() const;
    IteratorFlags iteratorFlags() const;
    void setTimeout(quint64 timeout);
    quint64 timeout() const;

    bool createEnumerator();
    [[nodiscard]] DFileFuture *createEnumeratorAsync(int ioPriority, QObject *parent = nullptr);

    bool hasNext() const;
    QUrl next() const;

    quint64 fileCount();
    [[nodiscard]] DFileInfo *createFileInfo(QObject *parent = nullptr) const;
    //!<@~english DError lastError() const; // TODO(lanxs): deal error

private:
    QScopedPointer<DEnumeratorPrivate> d;
};
DIO_END_NAMESPACE

#endif   // DENUMERATOR_H
