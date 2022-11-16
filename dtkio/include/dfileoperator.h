// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFILEOPERATOR_H
#define DFILEOPERATOR_H

#include <QUrl>
#include <QScopedPointer>

#include "dtkio_global.h"
#include "dtkiotypes.h"

DIO_BEGIN_NAMESPACE
class DFileFuture;
class DFileOperatorPrivate;
class DFileOperator
{
public:
    explicit DFileOperator(const QUrl &url);
    ~DFileOperator();

    QUrl url() const;
    bool renameFile(const QString &newName);
    bool copyFile(const QUrl &destUrl, CopyFlag flag);
    bool moveFile(const QUrl &destUrl, CopyFlag flag);
    bool trashFile();
    bool deleteFile();
    bool touchFile();
    bool makeDirectory();
    bool createLink(const QUrl &link);
    //!<@~english DError lastError() const; // TODO(lanxs): deal error

    [[nodiscard]] DFileFuture *renameFileAsync(const QString &newName, int ioPriority, QObject *parent = nullptr);
    [[nodiscard]] DFileFuture *copyFileAsync(const QUrl &destUrl, CopyFlag flag, int ioPriority, QObject *parent = nullptr);
    [[nodiscard]] DFileFuture *moveFileAsync(const QUrl &destUrl, CopyFlag flag, int ioPriority, QObject *parent = nullptr);
    [[nodiscard]] DFileFuture *trashFileAsync(int ioPriority, QObject *parent = nullptr);
    [[nodiscard]] DFileFuture *deleteFileAsync(int ioPriority, QObject *parent = nullptr);
    [[nodiscard]] DFileFuture *restoreFile(QObject *parent = nullptr);
    [[nodiscard]] DFileFuture *restoreFileAsync(int ioPriority, QObject *parent = nullptr);
    [[nodiscard]] DFileFuture *touchFileAsync(int ioPriority, QObject *parent = nullptr);
    [[nodiscard]] DFileFuture *makeDirectoryAsync(int ioPriority, QObject *parent = nullptr);
    [[nodiscard]] DFileFuture *createLinkAsync(const QUrl &link, int ioPriority, QObject *parent = nullptr);

private:
    QScopedPointer<DFileOperatorPrivate> d;
};
DIO_END_NAMESPACE

#endif   // DFILEOPERATOR_H
