// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFILEOPERATOR_P_H
#define DFILEOPERATOR_P_H

#include <QUrl>

#include <DError>

#include "dtkio_global.h"
#include "dfileerror.h"

DCORE_USE_NAMESPACE
DIO_BEGIN_NAMESPACE
class DFileOperator;
class DFileOperatorPrivate
{
public:
    explicit DFileOperatorPrivate(DFileOperator *q);
    ~DFileOperatorPrivate();

    void setError(IOErrorCode code);

    DFileOperator *q = nullptr;
    QUrl url;
    DError error { IOErrorCode::NoError, IOErrorMessage(IOErrorCode::NoError) };
};
DIO_END_NAMESPACE

#endif   // DFILEOPERATOR_P_H
