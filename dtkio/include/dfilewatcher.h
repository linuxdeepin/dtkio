// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFILEWATCHER_H
#define DFILEWATCHER_H

#include <QObject>
#include <QUrl>
#include <QScopedPointer>

#include "dtkio_global.h"
#include "dtkiotypes.h"

DIO_BEGIN_NAMESPACE
class DFileFuture;
class DFileWatcherPrivate;
class DFileWatcher : public QObject
{
    Q_OBJECT
public:
    explicit DFileWatcher(const QUrl &url, QObject *parent = nullptr);
    ~DFileWatcher();

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
    //DError lastError() const; // TODO(lanxs): deal error

Q_SIGNALS:
    void fileChanged(const QUrl &url);
    void fileDeleted(const QUrl &url);
    void fileAdded(const QUrl &url);
    void fileRenamed(const QUrl &fromUrl, const QUrl &toUrl);

private:
    QScopedPointer<DFileWatcherPrivate> d;
};
DIO_END_NAMESPACE

#endif   // DFILEWATCHER_H
