// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFILE_P_H
#define DFILE_P_H

#include <QUrl>

#include "dtkio_global.h"

DIO_BEGIN_NAMESPACE
class DFile;
class DFilePrivate
{
public:
    explicit DFilePrivate(DFile *q);
    ~DFilePrivate();

    DFile *q = nullptr;
};
DIO_END_NAMESPACE

#endif   // DFILE_P_H
