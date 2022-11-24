// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFILEMONITOR_H
#define DFILEMONITOR_H

#include <QObject>
#include <QUrl>
#include <QScopedPointer>

#include <DError>

#include "dtkio_global.h"
#include "dtkiotypes.h"

DCORE_USE_NAMESPACE
DIO_BEGIN_NAMESPACE
class DFileFuture;
class DFileMonitorPrivate;
class DFileMonitor : public QObject
{
    Q_OBJECT
public:
    explicit DFileMonitor(const QUrl &url, QObject *parent = nullptr);
    ~DFileMonitor();

    QUrl url() const;
    void setTimeRate(quint32 msec);
    quint32 timeRate() const;
    void setWatchType(WatchType type);
    WatchType watchType() const;
    bool running() const;
    bool start();
    bool start(quint32 timeRate = 200);
    [[nodiscard]] DFileFuture *startAsync(int ioPriority, QObject *parent = nullptr);

    bool stop();

    DError lastError() const;

Q_SIGNALS:
    void fileChanged(const QUrl &url);
    void fileDeleted(const QUrl &url);
    void fileAdded(const QUrl &url);
    void fileRenamed(const QUrl &fromUrl, const QUrl &toUrl);

private:
    QScopedPointer<DFileMonitorPrivate> d;
};
DIO_END_NAMESPACE

#endif   // DFILEMONITOR_H
