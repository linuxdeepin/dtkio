// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "doptifcaldiscmanager.h"

DBURN_BEGIN_NAMESPACE

using DTK_CORE_NAMESPACE::DExpected;

DExpected<DOpticalDiscInfo *> DOpticalDiscManager::createOpticalDiscInfo(const QString &dev, QObject *parent)
{
    // TODO(zhangs): impl me!
    return {};
}

DExpected<DOpticalDiscOperator *> DOpticalDiscManager::createOpticalDiscOperator(const QString &dev, QObject *parent)
{
    // TODO(zhangs): impl me!
    return {};
}

DBURN_END_NAMESPACE
