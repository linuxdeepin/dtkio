// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFILE_H
#define DFILE_H

#include <QObject>
#include <QUrl>
#include <QScopedPointer>

#include "dtkio_global.h"
#include "dtkiotypes.h"

DIO_BEGIN_NAMESPACE
class DFileFuture;
class DFilePrivate;
class DFile
{
public:
    explicit DFile(const QUrl &url);
    ~DFile();

    QUrl url() const;
    bool open(OpenFlags mode);
    bool close();
    bool isOpen() const;

    qint64 read(QByteArray &data, qint64 maxSize);
    QByteArray readAll();
    qint64 write(const QByteArray &data, qint64 len);
    qint64 write(const QByteArray &data);

    bool seek(qint64 pos, SeekType type = SeekType::Begin) const;
    qint64 pos() const;
    bool flush();
    qint64 size() const;
    bool exists() const;

    Permissions permissions() const;
    bool setPermissions(Permissions permission);
    bool setAttribute(AttributeID id, const QVariant &value);
    bool setCustomAttribute(const QByteArray &key, const void *value, const DFileAttributeType type, const FileQueryInfoFlags flag = FileQueryInfoFlags::TypeNone);
    //DError lastError() const; // TODO(lanxs): deal error

    [[nodiscard]] DFileFuture *openAsync(OpenFlags mode, int ioPriority, QObject *parent = nullptr);
    [[nodiscard]] DFileFuture *closeAsync(int ioPriority, QObject *parent = nullptr);
    [[nodiscard]] DFileFuture *readAsync(qint64 maxSize, int ioPriority, QObject *parent = nullptr);
    [[nodiscard]] DFileFuture *readAllAsync(int ioPriority, QObject *parent = nullptr);
    [[nodiscard]] DFileFuture *writeAsync(const QByteArray &data, qint64 len, int ioPriority, QObject *parent = nullptr);
    [[nodiscard]] DFileFuture *writeAsync(const QByteArray &data, int ioPriority, QObject *parent = nullptr);
    [[nodiscard]] DFileFuture *flushAsync(int ioPriority, QObject *parent = nullptr);
    [[nodiscard]] DFileFuture *sizeAsync(int ioPriority, QObject *parent = nullptr);
    [[nodiscard]] DFileFuture *existsAsync(int ioPriority, QObject *parent = nullptr);
    [[nodiscard]] DFileFuture *permissionsAsync(int ioPriority, QObject *parent = nullptr);
    [[nodiscard]] DFileFuture *setPermissionsAsync(Permissions permission, int ioPriority, QObject *parent = nullptr);

private:
    QScopedPointer<DFilePrivate> d;
};
DIO_END_NAMESPACE

#endif   // DFILE_H
