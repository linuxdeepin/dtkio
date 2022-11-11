// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DENUMERATOR_P_H
#define DENUMERATOR_P_H

#include <QUrl>

#include "dtkio_global.h"

DIO_BEGIN_NAMESPACE
class DEnumerator;
class DEnumeratorPrivate
{
public:
    explicit DEnumeratorPrivate(DEnumerator *q);
    ~DEnumeratorPrivate();

    DEnumerator *q = nullptr;
};
DIO_END_NAMESPACE

#endif   // DENUMERATOR_P_H
