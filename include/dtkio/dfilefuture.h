// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFILEFUTURE_H
#define DFILEFUTURE_H

#include <QObject>
#include <QScopedPointer>

#include "dtkio_global.h"

DIO_BEGIN_NAMESPACE
class DFuturePrivate;
class DFileFuture : public QObject
{
    Q_OBJECT
public:
    explicit DFileFuture(QObject *parent = nullptr);
    ~DFileFuture();

    bool cancel();
    bool isFinished() const;
    bool hasError() const;

Q_SIGNALS:
    void finished();
    void progressNotify(qint64 current, qint64 total);

private:
    QScopedPointer<DFuturePrivate> d;
};
DIO_END_NAMESPACE

#endif   // DFILEFUTURE_H
