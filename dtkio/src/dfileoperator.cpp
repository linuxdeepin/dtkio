// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dfileoperator_p.h"
#include "dfileoperator.h"

DIO_BEGIN_NAMESPACE

DFileOperatorPrivate::DFileOperatorPrivate(DFileOperator *q)
{
}

DFileOperatorPrivate::~DFileOperatorPrivate()
{
}

DFileOperator::DFileOperator(const QUrl &url)
{
}

DFileOperator::~DFileOperator()
{
}

QUrl DFileOperator::url() const
{
    return QUrl();
}

bool DFileOperator::renameFile(const QString &newName)
{
    return false;
}

bool DFileOperator::copyFile(const QUrl &destUrl, CopyFlag flag)
{
    return false;
}

bool DFileOperator::moveFile(const QUrl &destUrl, CopyFlag flag)
{
    return false;
}

bool DFileOperator::trashFile()
{
    return false;
}

bool DFileOperator::deleteFile()
{
    return false;
}

bool DFileOperator::touchFile()
{
    return false;
}

bool DFileOperator::makeDirectory()
{
    return false;
}

bool DFileOperator::createLink(const QUrl &link)
{
    return false;
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

DFileFuture *DFileOperator::restoreFile(QObject *parent)
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
