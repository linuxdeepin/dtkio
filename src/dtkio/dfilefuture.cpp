// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dfilefuture_p.h"
#include "dfilefuture.h"

DIO_BEGIN_NAMESPACE

DFuturePrivate::DFuturePrivate(DFileFuture *q)
{
}

DFuturePrivate::~DFuturePrivate()
{
}

DFileFuture::DFileFuture(QObject *parent)
    : QObject(parent)
{
}

DFileFuture::~DFileFuture()
{
}

bool DFileFuture::cancel()
{
    return false;
}

bool DFileFuture::isFinished() const
{
    return false;
}

bool DFileFuture::hasError() const
{
    return false;
}

DIO_END_NAMESPACE
