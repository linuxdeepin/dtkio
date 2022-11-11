// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFILEOPERATOR_P_H
#define DFILEOPERATOR_P_H

#include <QUrl>

#include "dtkio_global.h"

DIO_BEGIN_NAMESPACE
class DFileOperator;
class DFileOperatorPrivate
{
public:
    explicit DFileOperatorPrivate(DFileOperator *q);
    ~DFileOperatorPrivate();

    DFileOperator *q = nullptr;
};
DIO_END_NAMESPACE

#endif   // DFILEOPERATOR_P_H
