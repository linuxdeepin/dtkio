// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFILE_P_H
#define DFILE_P_H

#include <QUrl>

#include <gio/gio.h>

#include <DError>

#include "dtkio_global.h"
#include "dtkiotypes.h"
#include "dfileerror.h"

DCORE_USE_NAMESPACE
DIO_BEGIN_NAMESPACE
class DFile;
class DFilePrivate
{
public:
    explicit DFilePrivate(DFile *q);
    ~DFilePrivate();

    bool exists();
    bool checkOpenFlags(OpenFlags *mode);
    GInputStream *inputStream();
    GOutputStream *outputStream();

    void setError(IOErrorCode code);

    DFile *q = nullptr;
    QUrl url;
    bool isOpen { false };
    DError error { IOErrorCode::NoError, IOErrorMessage(IOErrorCode::NoError) };

    GIOStream *ioStream = nullptr;
    GInputStream *iStream = nullptr;
    GOutputStream *oStream = nullptr;
};
DIO_END_NAMESPACE

#endif   // DFILE_P_H
