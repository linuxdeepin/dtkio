// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dfile_p.h"

#include <DFile>

DIO_BEGIN_NAMESPACE

DFilePrivate::DFilePrivate(DFile *q)
{
}

DFilePrivate::~DFilePrivate()
{
}

DFile::DFile(const QUrl &url)
{
}

DFile::~DFile()
{
}

QUrl DFile::url() const
{
    return QUrl();
}

bool DFile::open(OpenFlags mode)
{
    return false;
}

bool DFile::close()
{
    return false;
}

bool DFile::isOpen() const
{
    return false;
}

qint64 DFile::read(QByteArray &data, qint64 maxSize)
{
    return -1;
}

QByteArray DFile::readAll()
{
    return QByteArray();
}

qint64 DFile::write(const QByteArray &data, qint64 len)
{
    return -1;
}

qint64 DFile::write(const QByteArray &data)
{
    return -1;
}

bool DFile::seek(qint64 pos, SeekType type) const
{
    return false;
}

qint64 DFile::pos() const
{
    return -1;
}

bool DFile::flush()
{
    return false;
}

qint64 DFile::size() const
{
    return -1;
}

bool DFile::exists() const
{
    return false;
}

Permissions DFile::permissions() const
{
    return Permission::NoPermission;
}

bool DFile::setPermissions(Permissions permission)
{
    return false;
}

bool DFile::setAttribute(AttributeID id, const QVariant &value)
{
    return false;
}

bool DFile::setCustomAttribute(const QByteArray &key, const void *value, const DFileAttributeType type, const FileQueryInfoFlags flag)
{
    return false;
}

DFileFuture *DFile::openAsync(OpenFlags mode, int ioPriority, QObject *parent)
{
    return nullptr;
}

DFileFuture *DFile::closeAsync(int ioPriority, QObject *parent)
{
    return nullptr;
}

DFileFuture *DFile::readAsync(qint64 maxSize, int ioPriority, QObject *parent)
{
    return nullptr;
}

DFileFuture *DFile::readAllAsync(int ioPriority, QObject *parent)
{
    return nullptr;
}

DFileFuture *DFile::writeAsync(const QByteArray &data, qint64 len, int ioPriority, QObject *parent)
{
    return nullptr;
}

DFileFuture *DFile::writeAsync(const QByteArray &data, int ioPriority, QObject *parent)
{
    return nullptr;
}

DFileFuture *DFile::flushAsync(int ioPriority, QObject *parent)
{
    return nullptr;
}

DFileFuture *DFile::sizeAsync(int ioPriority, QObject *parent)
{
    return nullptr;
}

DFileFuture *DFile::existsAsync(int ioPriority, QObject *parent)
{
    return nullptr;
}

DFileFuture *DFile::permissionsAsync(int ioPriority, QObject *parent)
{
    return nullptr;
}

DFileFuture *DFile::setPermissionsAsync(Permissions permission, int ioPriority, QObject *parent)
{
    return nullptr;
}

DIO_END_NAMESPACE
