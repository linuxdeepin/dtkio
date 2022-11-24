// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dfileinfo_p.h"
#include "dfileinfo.h"

#include <sys/stat.h>

#include <gio/gio.h>

#include "dfilehelper.h"

DIO_BEGIN_NAMESPACE

DFileInfoPrivate::DFileInfoPrivate(DFileInfo *fileInfo)
    : q(fileInfo)
{
}

DFileInfoPrivate::~DFileInfoPrivate()
{
}

bool DFileInfoPrivate::initQuerier()
{
    const char *attributes = queryAttributes;
    const FileQueryInfoFlags flags = queryInfoFlags;
    g_autoptr(GCancellable) cancellabel = g_cancellable_new();
    g_autoptr(GError) gerror = nullptr;
    g_autoptr(GFile) gfile = DFileHelper::fileNewForUrl(url);

    GFileInfo *fileinfo = g_file_query_info(gfile, attributes, GFileQueryInfoFlags(flags), cancellabel, &gerror);
    if (gerror)
        setError(IOErrorCode(gerror->code));
    if (!fileinfo)
        return false;

    if (gFileInfo) {
        g_object_unref(gFileInfo);
        gFileInfo = nullptr;
    }
    gFileInfo = fileinfo;
    querierInit = true;

    return true;
}

bool DFileInfoPrivate::checkQuerier()
{
    if (!querierInit)
        return initQuerier();

    return true;
}

void DFileInfoPrivate::setError(IOErrorCode code)
{
    error.setErrorCode(code);
    error.setErrorMessage(IOErrorMessage(code));
}

DFileInfo::DFileInfo(const QUrl &url, QObject *parent)
    : QObject(parent), d(new DFileInfoPrivate(this))
{
    d->url = url;
}

DFileInfo::~DFileInfo()
{
}

QUrl DFileInfo::url() const
{
    return d->url;
}

void DFileInfo::setQueryAttributes(const QByteArray &attributes)
{
    d->queryAttributes = attributes;
}

void DFileInfo::setQueryInfoFlags(const FileQueryInfoFlags flags)
{
    d->queryInfoFlags = flags;
}

QByteArray DFileInfo::queryAttributes() const
{
    return d->queryAttributes;
}

FileQueryInfoFlags DFileInfo::queryInfoFlag() const
{
    return d->queryInfoFlags;
}

bool DFileInfo::initQuerier()
{
    return d->initQuerier();
}

QVariant DFileInfo::attribute(AttributeID id, bool *success) const
{
    if (!d->checkQuerier()) {
        if (success)
            *success = false;
        return DFileHelper::attributeDefaultValue(id);
    }

    const QVariant &value = DFileHelper::attributeFromInfo(id, d->gFileInfo, d->url);
    if (success)
        *success = value.isValid();

    return value;
}

QVariant DFileInfo::attribute(const QByteArray &key, const AttributeType type) const
{
    if (!d->checkQuerier())
        return QVariant();

    const QVariant &value = DFileHelper::attribute(key, type, d->gFileInfo);
    return value;
}

bool DFileInfo::hasAttribute(AttributeID id) const
{
    if (!d->checkQuerier())
        return false;

    const std::string &key = DFileHelper::attributeKey(id);
    return g_file_info_has_attribute(d->gFileInfo, key.c_str());
}

bool DFileInfo::hasAttribute(const QByteArray &key) const
{
    if (!d->checkQuerier())
        return false;

    return g_file_info_has_attribute(d->gFileInfo, key);
}

bool DFileInfo::exists() const
{
    if (!d->checkQuerier())
        return false;

    return g_file_info_get_file_type(d->gFileInfo) != G_FILE_TYPE_UNKNOWN;
}

bool DFileInfo::refresh()
{
    return d->initQuerier();
}

Permissions DFileInfo::permissions() const
{
    Permissions retValue = Permission::NoPermission;

    if (!d->checkQuerier())
        return retValue;

    const QVariant &value = DFileHelper::attributeFromInfo(AttributeID::UnixMode, d->gFileInfo);
    if (!value.isValid())
        return retValue;
    const uint32_t stMode = value.toUInt();

    if ((stMode & S_IXUSR) == S_IXUSR) {
        retValue |= Permission::ExeOwner;
        retValue |= Permission::ExeUser;
    }
    if ((stMode & S_IWUSR) == S_IWUSR) {
        retValue |= Permission::WriteOwner;
        retValue |= Permission::WriteUser;
    }
    if ((stMode & S_IRUSR) == S_IRUSR) {
        retValue |= Permission::ReadOwner;
        retValue |= Permission::ReadUser;
    }

    if ((stMode & S_IXGRP) == S_IXGRP)
        retValue |= Permission::ExeGroup;
    if ((stMode & S_IWGRP) == S_IWGRP)
        retValue |= Permission::WriteGroup;
    if ((stMode & S_IRGRP) == S_IRGRP)
        retValue |= Permission::ReadGroup;

    if ((stMode & S_IXOTH) == S_IXOTH)
        retValue |= Permission::ExeOther;
    if ((stMode & S_IWOTH) == S_IWOTH)
        retValue |= Permission::WriteOther;
    if ((stMode & S_IROTH) == S_IROTH)
        retValue |= Permission::ReadOther;

    return retValue;
}

DFileFuture *DFileInfo::initQuerierAsync(int ioPriority, QObject *parent)
{
    return nullptr;
}

DFileFuture *DFileInfo::attributeAsync(AttributeID id, int ioPriority, QObject *parent) const
{
    return nullptr;
}

DFileFuture *DFileInfo::attributeAsync(const QByteArray &key, const AttributeType type, int ioPriority, QObject *parent) const
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

DError DFileInfo::lastError() const
{
    return d->error;
}

DIO_END_NAMESPACE
