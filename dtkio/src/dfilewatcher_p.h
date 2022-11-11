// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFILEWATCHER_P_H
#define DFILEWATCHER_P_H

#include <QUrl>

#include "dtkio_global.h"

DIO_BEGIN_NAMESPACE
class DFileWatcher;
class DFileWatcherPrivate
{
public:
    explicit DFileWatcherPrivate(DFileWatcher *q);
    ~DFileWatcherPrivate();

    DFileWatcher *q = nullptr;
};
DIO_END_NAMESPACE

#endif   // DFILEWATCHER_P_H
