// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFILEINFO_P_H
#define DFILEINFO_P_H

#include <QUrl>

#include "dtkio_global.h"

DIO_BEGIN_NAMESPACE
class DFileInfo;
class DFileInfoPrivate
{
public:
    explicit DFileInfoPrivate(DFileInfo *q);
    ~DFileInfoPrivate();

    DFileInfo *q = nullptr;
};
DIO_END_NAMESPACE

#endif   // DFILEINFO_P_H
