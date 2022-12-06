// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dopticaldiscoperator.h"

DBURN_BEGIN_NAMESPACE

using DTK_CORE_NAMESPACE::DExpected;

DOpticalDiscOperator::~DOpticalDiscOperator()
{
}

void DOpticalDiscOperator::setSpeed(int speed)
{
    // TODO(zhangs): impl me!
}

void DOpticalDiscOperator::setVolumeName(const QString &name)
{
    // TODO(zhangs): impl me!
}

void DOpticalDiscOperator::setCheckPrecision(double precision)
{
    // TODO(zhangs): impl me!
}

DExpected<bool> DOpticalDiscOperator::burn(const QString &stagePath, const BurnOptions &opts)
{
    // TODO(zhangs): impl me!
    return {};
}

Dtk::Core::DExpected<bool> DOpticalDiscOperator::erase()
{
    // TODO(zhangs): impl me!
    return {};
}

Dtk::Core::DExpected<bool> DOpticalDiscOperator::checkmedia()
{
    // TODO(zhangs): impl me!
    return {};
}

Dtk::Core::DExpected<bool> DOpticalDiscOperator::writeISO(const QString &isoPath)
{
    // TODO(zhangs): impl me!
    return {};
}

Dtk::Core::DExpected<bool> DOpticalDiscOperator::dumpISO(const QString &isoPath)
{
    // TODO(zhangs): impl me!
    return {};
}

DOpticalDiscOperator::DOpticalDiscOperator(QObject *parent)
    : QObject(parent)
{
}

DBURN_END_NAMESPACE
