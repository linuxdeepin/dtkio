// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dfileinfo_p.h"

#include <DFileInfo>

DIO_BEGIN_NAMESPACE

DFileInfoPrivate::DFileInfoPrivate(DFileInfo *q)
{
}

DFileInfoPrivate::~DFileInfoPrivate()
{
}

DFileInfo::DFileInfo(const QUrl &url, QObject *parent)
    : QObject(parent)
{
}

DFileInfo::~DFileInfo()
{
}

QUrl DFileInfo::url() const
{
    return QUrl();
}

void DFileInfo::setQueryAttributes(const QByteArray &attributes)
{
}

void DFileInfo::setQueryInfoFlags(const FileQueryInfoFlags flag)
{
}

QByteArray DFileInfo::queryAttributes() const
{
    return QByteArray();
}

FileQueryInfoFlags DFileInfo::queryInfoFlag() const
{
    return FileQueryInfoFlags::TypeNone;
}

bool DFileInfo::initQuerier()
{
    return false;
}

QVariant DFileInfo::attribute(AttributeID id, bool *success) const
{
    return QVariant();
}

QVariant DFileInfo::attribute(const QByteArray &key, const DFileAttributeType type) const
{
    return QVariant();
}

bool DFileInfo::hasAttribute(AttributeID id) const
{
    return false;
}

bool DFileInfo::hasAttribute(const QByteArray &key) const
{
    return false;
}

bool DFileInfo::exists() const
{
    return false;
}

bool DFileInfo::refresh()
{
    return false;
}

Permissions DFileInfo::permissions() const
{
    return Permission::NoPermission;
}

DFileFuture *DFileInfo::initQuerierAsync(int ioPriority, QObject *parent)
{
    return nullptr;
}

DFileFuture *DFileInfo::attributeAsync(AttributeID id, int ioPriority, QObject *parent) const
{
    return nullptr;
}

DFileFuture *DFileInfo::attributeAsync(const QByteArray &key, const DFileAttributeType type, int ioPriority, QObject *parent) const
{
    return nullptr;
}

DFileFuture *DFileInfo::existsAsync(int ioPriority, QObject *parent) const
{
    return nullptr;
}

DFileFuture *DFileInfo::refreshAsync(int ioPriority, QObject *parent)
{
    return nullptr;
}

DFileFuture *DFileInfo::permissionsAsync(int ioPriority, QObject *parent)
{
    return nullptr;
}

DIO_END_NAMESPACE
