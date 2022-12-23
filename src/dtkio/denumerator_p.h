// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DENUMERATOR_P_H
#define DENUMERATOR_P_H

#include <QUrl>
#include <QStack>

#include <gio/gio.h>

#include <DError>

#include "dtkio_global.h"
#include "dtkiotypes.h"
#include "dfileerror.h"

DCORE_USE_NAMESPACE
DIO_BEGIN_NAMESPACE
class DFileInfo;
class DEnumerator;
class DEnumeratorPrivate
{
public:
    explicit DEnumeratorPrivate(DEnumerator *q);
    ~DEnumeratorPrivate();

    void setError(IOErrorCode code);
    bool createEnumerator(const QUrl &url);
    bool checkFilter();

    DEnumerator *q = nullptr;
    QUrl url;
    QStringList nameFilters;
    DirFilters dirFilters;
    IteratorFlags iteratorflags;
    quint64 timeout = 0;
    DError error { IOErrorCode::NoError, IOErrorMessage(IOErrorCode::NoError) };
    bool enumeratorInited = false;

    QStack<GFileEnumerator *> stackEnumerator;
    QUrl nextUrl;
    DFileInfo *fileInfo = nullptr;
    QMap<QUrl, QSet<QString>> hideListMap;
};
DIO_END_NAMESPACE

#endif   // DENUMERATOR_P_H
