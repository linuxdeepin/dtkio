// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dfileoperator_p.h"
#include "dfileoperator.h"

#include <gio/gio.h>

#include "dfilehelper.h"
#include "dfileinfo.h"

DIO_BEGIN_NAMESPACE

DFileOperatorPrivate::DFileOperatorPrivate(DFileOperator *fileOperator)
    : q(fileOperator)
{
}

DFileOperatorPrivate::~DFileOperatorPrivate()
{
}

void DFileOperatorPrivate::setError(IOErrorCode code)
{
    error.setErrorCode(code);
    error.setErrorMessage(IOErrorMessage(code));
}

DFileOperator::DFileOperator(const QUrl &url)
    : d(new DFileOperatorPrivate(this))
{
    d->url = url;
}

DFileOperator::~DFileOperator()
{
}

QUrl DFileOperator::url() const
{
    return d->url;
}

bool DFileOperator::renameFile(const QString &newName)
{
    const QUrl &url = d->url;

    // name must deep copy, otherwise name freed and crash
    g_autofree gchar *name = g_strdup(newName.toStdString().c_str());

    g_autoptr(GFile) gfile = DFileHelper::fileNewForUrl(url);

    g_autoptr(GCancellable) cancellable = g_cancellable_new();
    g_autoptr(GError) gerror = nullptr;
    g_autoptr(GFile) gfileNew = g_file_set_display_name(gfile, name, cancellable, &gerror);

    if (!gfileNew) {
        d->setError(IOErrorCode(gerror->code));
        return false;
    }

    return true;
}

bool DFileOperator::copyFile(const QUrl &destUrl, CopyFlag flag)
{
    g_autoptr(GCancellable) cancellable = g_cancellable_new();
    g_autoptr(GError) gerror = nullptr;

    g_autoptr(GFile) gfileFrom = DFileHelper::fileNewForUrl(d->url);
    g_autoptr(GFile) gfileTo = DFileHelper::fileNewForUrl(destUrl);

    bool ret = g_file_copy(gfileFrom, gfileTo, GFileCopyFlags(flag), cancellable, nullptr, nullptr, &gerror);

    if (gerror) {
        d->setError(IOErrorCode(gerror->code));
        return false;
    }

    return ret;
}

bool DFileOperator::moveFile(const QUrl &destUrl, CopyFlag flag)
{
    g_autoptr(GCancellable) cancellable = g_cancellable_new();
    g_autoptr(GError) gerror = nullptr;

    g_autoptr(GFile) gfileFrom = DFileHelper::fileNewForUrl(d->url);
    g_autoptr(GFile) gfileTo = DFileHelper::fileNewForUrl(destUrl);

    bool ret = g_file_move(gfileFrom, gfileTo, GFileCopyFlags(flag), cancellable, nullptr, nullptr, &gerror);

    if (gerror)
        d->setError(IOErrorCode(gerror->code));

    return ret;
}

bool DFileOperator::trashFile()
{
    g_autoptr(GCancellable) cancellable = g_cancellable_new();
    g_autoptr(GError) gerror = nullptr;

    g_autoptr(GFile) gfile = DFileHelper::fileNewForUrl(d->url);

    bool ret = g_file_trash(gfile, cancellable, &gerror);

    if (gerror)
        d->setError(IOErrorCode(gerror->code));

    return ret;
}

bool DFileOperator::deleteFile()
{
    g_autoptr(GCancellable) cancellable = g_cancellable_new();
    g_autoptr(GError) gerror = nullptr;

    g_autoptr(GFile) gfile = DFileHelper::fileNewForUrl(d->url);

    bool ret = g_file_delete(gfile, cancellable, &gerror);

    if (gerror)
        d->setError(IOErrorCode(gerror->code));

    return ret;
}

bool DFileOperator::touchFile()
{
    g_autoptr(GCancellable) cancallable = g_cancellable_new();
    g_autoptr(GError) gerror = nullptr;

    g_autoptr(GFile) gfile = DFileHelper::fileNewForUrl(d->url);

    // if file exist, return failed
    g_autoptr(GFileOutputStream) stream = g_file_create(gfile, GFileCreateFlags::G_FILE_CREATE_NONE, cancallable, &gerror);

    if (gerror)
        d->setError(IOErrorCode(gerror->code));

    return stream != nullptr;
}

bool DFileOperator::makeDirectory()
{
    g_autoptr(GCancellable) cancellable = g_cancellable_new();
    g_autoptr(GError) gerror = nullptr;

    g_autoptr(GFile) gfile = DFileHelper::fileNewForUrl(d->url);

    bool ret = g_file_make_directory(gfile, cancellable, &gerror);

    if (gerror)
        d->setError(IOErrorCode(gerror->code));

    return ret;
}

bool DFileOperator::createLink(const QUrl &link)
{
    g_autoptr(GCancellable) cancellabel = g_cancellable_new();
    g_autoptr(GError) gerror = nullptr;

    g_autoptr(GFile) gfile = DFileHelper::fileNewForUrl(link);

    const QString &linkValue = d->url.toLocalFile();
    bool ret = g_file_make_symbolic_link(gfile, linkValue.toStdString().c_str(), cancellabel, &gerror);

    if (gerror)
        d->setError(IOErrorCode(gerror->code));

    return ret;
}

bool DFileOperator::restoreFile()
{
    DFileInfo dfileinfo(d->url);
    dfileinfo.setQueryAttributes(DFileHelper::attributeKey(AttributeID::TrashOrigPath).c_str());
    bool succ = dfileinfo.initQuerier();
    if (!succ)
        return false;
    if (!dfileinfo.hasAttribute(AttributeID::TrashOrigPath))
        return false;
    const QString &path = dfileinfo.attribute(AttributeID::TrashOrigPath).toString();
    if (path.isEmpty())
        return false;

    return moveFile(QUrl::fromLocalFile(path), CopyFlag::None);
}

DError DFileOperator::lastError() const
{
    return d->error;
}

DFileFuture *DFileOperator::renameFileAsync(const QString &newName, int ioPriority, QObject *parent)
{
    return nullptr;
}

DFileFuture *DFileOperator::copyFileAsync(const QUrl &destUrl, CopyFlag flag, int ioPriority, QObject *parent)
{
    return nullptr;
}

DFileFuture *DFileOperator::moveFileAsync(const QUrl &destUrl, CopyFlag flag, int ioPriority, QObject *parent)
{
    return nullptr;
}

DFileFuture *DFileOperator::trashFileAsync(int ioPriority, QObject *parent)
{
    return nullptr;
}

DFileFuture *DFileOperator::deleteFileAsync(int ioPriority, QObject *parent)
{
    return nullptr;
}

DFileFuture *DFileOperator::restoreFileAsync(int ioPriority, QObject *parent)
{
    return nullptr;
}

DFileFuture *DFileOperator::touchFileAsync(int ioPriority, QObject *parent)
{
    return nullptr;
}

DFileFuture *DFileOperator::makeDirectoryAsync(int ioPriority, QObject *parent)
{
    return nullptr;
}

DFileFuture *DFileOperator::createLinkAsync(const QUrl &link, int ioPriority, QObject *parent)
{
    return nullptr;
}

DIO_END_NAMESPACE
