// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFILEFUTURE_H
#define DFILEFUTURE_H

#include <QObject>
#include <QScopedPointer>

#include "dtkio_global.h"
#include "dtkiotypes.h"
#include "dfileerror.h"

DIO_BEGIN_NAMESPACE
class DFuturePrivate;
class DFileFuture : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DFileFuture)
public:
    explicit DFileFuture(QObject *parent = nullptr);
    ~DFileFuture();

    bool cancel();
    bool isFinished() const;
    bool hasError() const;
    void setError(IOErrorCode error);

Q_SIGNALS:
    void finished();
    void progressNotify(qint64 current, qint64 total);
    void infoAttribute(AttributeID id, const QVariant &value);
    void infoAttribute(const QByteArray &key, const QVariant &value);
    void infoExists(const bool exists);
    void infoPermissions(const Permissions permissions);
    void readData(const QByteArray &data);
    void writeAsyncSize(const qint64 &size);
    void infoSize(const quint64 &size);

private:
    QScopedPointer<DFuturePrivate> d;
};
DIO_END_NAMESPACE

#endif   // DFILEFUTURE_H
