// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dopticaldiscinfo.h"

DBURN_BEGIN_NAMESPACE

DOpticalDiscInfo::DOpticalDiscInfo(const DOpticalDiscInfo &info)
{
    // TODO(zhangs): impl me!
}

DOpticalDiscInfo &DOpticalDiscInfo::operator=(const DOpticalDiscInfo &info)
{
    // TODO(zhangs): impl me!
    return *this;
}

DOpticalDiscInfo::~DOpticalDiscInfo()
{
}

bool DOpticalDiscInfo::blank() const
{
    // TODO(zhangs): impl me!
    return {};
}

QString DOpticalDiscInfo::device() const
{
    // TODO(zhangs): impl me!
    return {};
}

QString DOpticalDiscInfo::volumeName() const
{
    // TODO(zhangs): impl me!
    return {};
}

quint64 DOpticalDiscInfo::usedSize() const
{
    // TODO(zhangs): impl me!
    return {};
}

quint64 DOpticalDiscInfo::availableSize() const
{
    // TODO(zhangs): impl me!
    return {};
}

quint64 DOpticalDiscInfo::totalSize() const
{
    // TODO(zhangs): impl me!
    return {};
}

quint64 DOpticalDiscInfo::dataBlocks() const
{
    // TODO(zhangs): impl me!
    return {};
}

MediaType DOpticalDiscInfo::mediaType() const
{
    // TODO(zhangs): impl me!
    return {};
}

QStringList DOpticalDiscInfo::writeSpeed() const
{
    // TODO(zhangs): impl me!
    return {};
}

DOpticalDiscInfo::DOpticalDiscInfo()
{
}

DOpticalDiscInfo::DOpticalDiscInfo(const QString &dev)
{
}

DBURN_END_NAMESPACE
