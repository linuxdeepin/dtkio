// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFILEINFO_P_H
#define DFILEINFO_P_H

#include <QUrl>

#include <gio/gio.h>

#include <DError>

#include "dtkio_global.h"
#include "dtkiotypes.h"
#include "dfileerror.h"

DCORE_USE_NAMESPACE
DIO_BEGIN_NAMESPACE
class DFileInfo;
class DFileInfoPrivate
{
public:
    explicit DFileInfoPrivate(DFileInfo *q);
    ~DFileInfoPrivate();

    bool initQuerier();
    bool checkQuerier();
    void setError(IOErrorCode code);

    DFileInfo *q = nullptr;
    QUrl url;
    QByteArray queryAttributes;
    FileQueryInfoFlags queryInfoFlags = FileQueryInfoFlags::TypeNone;
    bool querierInit = false;
    GFileInfo *gFileInfo = nullptr;

    DError error { IOErrorCode::NoError, IOErrorMessage(IOErrorCode::NoError) };
};
DIO_END_NAMESPACE

#endif   // DFILEINFO_P_H
