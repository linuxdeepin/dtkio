// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dfile_p.h"
#include "dfile.h"

#include <sys/stat.h>

#include <gio/gio.h>

#include "dfileerror.h"
#include "dfilehelper.h"

DIO_BEGIN_NAMESPACE

DFilePrivate::DFilePrivate(DFile *file)
    : q(file)
{
}

DFilePrivate::~DFilePrivate()
{
}

bool DFilePrivate::exists()
{
    g_autoptr(GFile) gfile = DFileHelper::fileNewForUrl(url);
    ;
    return g_file_query_file_type(gfile, G_FILE_QUERY_INFO_NONE, nullptr) != G_FILE_TYPE_UNKNOWN;
}

bool DFilePrivate::checkOpenFlags(OpenFlags *modeIn)
{
    OpenFlags &mode = *modeIn;

    if (mode & OpenFlag::NewOnly) {
        if (exists()) {
            setError(IOErrorCode::OpenFlagError);
            return false;
        }
    }

    if (mode & OpenFlag::ExistingOnly) {
        if (!exists()) {
            setError(IOErrorCode::OpenFlagError);
            return false;
        }
    }
    if ((mode & OpenFlag::NewOnly) && (mode & OpenFlag::ExistingOnly)) {
        setError(IOErrorCode::OpenFlagError);
        return false;
    }

    // WriteOnly implies Truncate when ReadOnly, Append, and NewOnly are not set.
    if ((mode & OpenFlag::WriteOnly) && !(mode & (uint16_t(OpenFlag::ReadOnly) | uint16_t(OpenFlag::Append) | uint16_t(OpenFlag::NewOnly))))
        mode |= OpenFlag::Truncate;

    if (mode & (uint16_t(OpenFlag::Append) | uint16_t(OpenFlag::NewOnly)))
        mode |= OpenFlag::WriteOnly;

    if ((mode & (uint16_t(OpenFlag::ReadOnly) | uint16_t(OpenFlag::WriteOnly))) == 0) {
        setError(IOErrorCode::OpenFlagError);
        return false;
    }

    if ((mode & OpenFlag::ExistingOnly) && !(mode & (uint16_t(OpenFlag::ReadOnly) | uint16_t(OpenFlag::WriteOnly)))) {
        setError(IOErrorCode::OpenFlagError);
        return false;
    }

    return true;
}

GInputStream *DFilePrivate::inputStream()
{
    if (iStream)
        return iStream;

    if (ioStream) {
        GInputStream *inputStream = g_io_stream_get_input_stream(ioStream);
        if (inputStream)
            return inputStream;
    }

    return nullptr;
}

GOutputStream *DFilePrivate::outputStream()
{
    if (oStream)
        return oStream;

    if (ioStream) {
        GOutputStream *outputStream = g_io_stream_get_output_stream(ioStream);
        if (outputStream)
            return outputStream;
    }

    return nullptr;
}

void DFilePrivate::setError(IOErrorCode code)
{
    error.setErrorCode(code);
    error.setErrorMessage(IOErrorMessage(code));
}

DFile::DFile(const QUrl &url)
    : d(new DFilePrivate(this))
{
    d->url = url;
}

DFile::~DFile()
{
}

QUrl DFile::url() const
{
    return d->url;
}

bool DFile::open(OpenFlags mode)
{
    if (d->isOpen) {
        d->setError(IOErrorCode::FileAlreadyOpened);
        return false;
    }

    if (!d->checkOpenFlags(&mode)) {
        return false;
    }

    g_autoptr(GFile) gfile = DFileHelper::fileNewForUrl(d->url);
    g_autoptr(GError) gerror = nullptr;
    g_autoptr(GCancellable) cancellable = g_cancellable_new();

    if (mode & OpenFlag::ReadOnly && !(mode & OpenFlag::WriteOnly)) {
        if (!exists()) {
            d->setError(IOErrorCode::FileNotFound);
            return false;
        }
        d->iStream = (GInputStream *)g_file_read(gfile, cancellable, &gerror);
        if (gerror)
            d->setError(IOErrorCode(gerror->code));

        if (!d->iStream) {
            return false;
        }
        return true;
    } else if (mode & OpenFlag::WriteOnly && !(mode & OpenFlag::ReadOnly)) {
        if (mode & OpenFlag::NewOnly) {
            d->oStream = (GOutputStream *)g_file_create(gfile, G_FILE_CREATE_NONE, cancellable, &gerror);
            if (gerror)
                d->setError(IOErrorCode(gerror->code));

            if (!d->oStream) {
                return false;
            }
        } else if (mode & OpenFlag::Append) {
            d->oStream = (GOutputStream *)g_file_append_to(gfile, G_FILE_CREATE_NONE, cancellable, &gerror);
            if (gerror)
                d->setError(IOErrorCode(gerror->code));

            if (!d->oStream) {
                return false;
            }
        } else {
            d->oStream = (GOutputStream *)g_file_replace(gfile, nullptr, false, G_FILE_CREATE_NONE, cancellable, &gerror);
            if (gerror)
                d->setError(IOErrorCode(gerror->code));

            if (!d->oStream) {
                return false;
            }
        }

        return true;
    } else if (mode & OpenFlag::ReadOnly && mode & OpenFlag::WriteOnly) {
        if (mode & OpenFlag::NewOnly) {
            d->ioStream = (GIOStream *)g_file_create_readwrite(gfile, G_FILE_CREATE_NONE, cancellable, &gerror);
            if (gerror)
                d->setError(IOErrorCode(gerror->code));

            if (!d->ioStream) {
                return false;
            }
        } else if (mode & OpenFlag::ExistingOnly) {
            d->ioStream = (GIOStream *)g_file_open_readwrite(gfile, cancellable, &gerror);
            if (gerror)
                d->setError(IOErrorCode(gerror->code));

            if (!d->ioStream) {
                return false;
            }
        } else {
            d->ioStream = (GIOStream *)g_file_replace_readwrite(gfile, nullptr, false, G_FILE_CREATE_NONE, cancellable, &gerror);
            if (gerror)
                d->setError(IOErrorCode(gerror->code));

            if (!d->ioStream) {
                return false;
            }
        }
        return true;
    } else {
        d->ioStream = (GIOStream *)g_file_replace_readwrite(gfile, nullptr, false, G_FILE_CREATE_NONE, cancellable, &gerror);
        if (gerror)
            d->setError(IOErrorCode(gerror->code));

        if (!d->ioStream) {
            return false;
        }
        return true;
    }
}

bool DFile::close()
{
    g_autoptr(GError) gerror = nullptr;
    g_autoptr(GCancellable) cancellable = g_cancellable_new();
    if (d->iStream) {
        if (!g_input_stream_is_closed(d->iStream))
            g_input_stream_close(d->iStream, cancellable, &gerror);
        g_object_unref(d->iStream);
        d->iStream = nullptr;
    }
    if (d->oStream) {
        if (!g_output_stream_is_closed(d->oStream))
            g_output_stream_close(d->oStream, cancellable, &gerror);
        g_object_unref(d->oStream);
        d->oStream = nullptr;
    }
    if (d->ioStream) {
        if (!g_io_stream_is_closed(d->ioStream))
            g_io_stream_close(d->ioStream, cancellable, &gerror);
        g_object_unref(d->ioStream);
        d->ioStream = nullptr;
    }
    return true;
}

bool DFile::isOpen() const
{
    return d->isOpen;
}

qint64 DFile::read(QByteArray &data, qint64 maxSize)
{
    GInputStream *inputStream = d->inputStream();
    if (!inputStream) {
        d->setError(IOErrorCode::OpenFailed);
        return -1;
    }

    char cdata[maxSize + 1];
    memset(&cdata, 0, maxSize + 1);

    g_autoptr(GCancellable) cancellable = g_cancellable_new();
    g_autoptr(GError) gerror = nullptr;
    gssize readSize = g_input_stream_read(inputStream,
                                          cdata,
                                          static_cast<gsize>(maxSize),
                                          cancellable,
                                          &gerror);
    if (gerror) {
        d->setError(IOErrorCode(gerror->code));
        return -1;
    }

    data.append(cdata, readSize);
    return readSize;
}

QByteArray DFile::readAll()
{
    GInputStream *inputStream = d->inputStream();
    if (!inputStream) {
        d->setError(IOErrorCode::OpenFailed);
        return QByteArray();
    }

    QByteArray dataRet;

    const gsize size = 8192;

    g_autoptr(GCancellable) cancellable = g_cancellable_new();
    g_autoptr(GError) gerror = nullptr;

    while (true) {
        gsize bytesRead;
        char data[size + 1];
        memset(data, 0, size + 1);

        gboolean read = g_input_stream_read_all(inputStream,
                                                data,
                                                size,
                                                &bytesRead,
                                                cancellable,
                                                &gerror);
        if (!read || gerror) {
            if (gerror)
                d->setError(IOErrorCode(gerror->code));
            break;
        }
        if (bytesRead == 0)
            break;

        dataRet.append(data, bytesRead);
    }

    return dataRet;
}

qint64 DFile::write(const QByteArray &data, qint64 len)
{
    GOutputStream *outputStream = d->outputStream();
    if (!outputStream) {
        d->setError(IOErrorCode::OpenFailed);
        return -1;
    }

    g_autoptr(GCancellable) cancellable = g_cancellable_new();
    g_autoptr(GError) gerror = nullptr;

    gssize write = g_output_stream_write(outputStream,
                                         data.constData(),
                                         gsize(len),
                                         cancellable,
                                         &gerror);
    if (gerror)
        d->setError(IOErrorCode(gerror->code));

    return write;
}

qint64 DFile::write(const QByteArray &data)
{
    return write(data, data.length());
}

bool DFile::seek(qint64 pos, SeekType type) const
{
    GInputStream *inputStream = d->inputStream();
    if (!inputStream) {
        d->setError(IOErrorCode::OpenFailed);
        return false;
    }

    // seems g_seekable_can_seek only support local file, survey after. todo lanxs
    gboolean canSeek = G_IS_SEEKABLE(inputStream) /*&& g_seekable_can_seek(G_SEEKABLE(inputStream))*/;
    if (!canSeek) {
        return false;
    }

    GSeekable *seekable = G_SEEKABLE(inputStream);
    if (!seekable) {
        return false;
    }

    bool ret = false;
    GSeekType gtype = G_SEEK_CUR;
    switch (type) {
    case SeekType::Begin:
        gtype = G_SEEK_SET;
        break;
    case SeekType::End:
        gtype = G_SEEK_END;
        break;

    default:
        break;
    }

    g_autoptr(GCancellable) cancellable = g_cancellable_new();
    g_autoptr(GError) gerror = nullptr;
    ret = g_seekable_seek(seekable, pos, gtype, cancellable, &gerror);
    if (gerror)
        d->setError(IOErrorCode(gerror->code));

    return ret;
}

qint64 DFile::pos() const
{
    GInputStream *inputStream = d->inputStream();
    if (!inputStream) {
        d->setError(IOErrorCode::OpenFailed);
        return -1;
    }

    gboolean canSeek = G_IS_SEEKABLE(inputStream);
    if (!canSeek) {
        return -1;
    }

    GSeekable *seekable = G_SEEKABLE(inputStream);
    if (!seekable) {
        return -1;
    }

    goffset pos = g_seekable_tell(seekable);

    return qint64(pos);
}

bool DFile::flush()
{
    GOutputStream *outputStream = d->outputStream();
    if (!outputStream) {
        d->setError(IOErrorCode::OpenFailed);
        return false;
    }

    g_autoptr(GCancellable) cancellable = g_cancellable_new();
    g_autoptr(GError) gerror = nullptr;
    gboolean ret = g_output_stream_flush(outputStream, cancellable, &gerror);

    if (gerror)
        d->setError(IOErrorCode(gerror->code));
    return ret;
}

qint64 DFile::size() const
{
    g_autoptr(GFile) gfile = DFileHelper::fileNewForUrl(d->url);

    g_autoptr(GCancellable) cancellable = g_cancellable_new();
    g_autoptr(GError) gerror = nullptr;
    const std::string &attributeKey = DFileHelper::attributeKey(AttributeID::StandardSize);
    g_autoptr(GFileInfo) fileInfo = g_file_query_info(gfile, attributeKey.c_str(), G_FILE_QUERY_INFO_NONE, cancellable, &gerror);

    if (gerror)
        d->setError(IOErrorCode(gerror->code));

    if (!fileInfo)
        return -1;

    const QVariant &value = DFileHelper::attributeFromInfo(AttributeID::StandardSize, fileInfo);
    if (!value.isValid())
        return DFileHelper::attributeDefaultValue(AttributeID::StandardSize).toLongLong();

    return value.toLongLong();
}

bool DFile::exists() const
{
    return d->exists();
}

Permissions DFile::permissions() const
{
    Permissions retValue = Permission::NoPermission;

    g_autoptr(GFile) gfile = DFileHelper::fileNewForUrl(d->url);

    g_autoptr(GCancellable) cancellable = g_cancellable_new();
    g_autoptr(GError) gerror = nullptr;
    const std::string &attributeKey = DFileHelper::attributeKey(AttributeID::UnixMode);
    g_autoptr(GFileInfo) fileInfo = g_file_query_info(gfile, attributeKey.c_str(), G_FILE_QUERY_INFO_NONE, cancellable, &gerror);

    if (gerror)
        d->setError(IOErrorCode(gerror->code));

    if (!fileInfo)
        return retValue;

    const QVariant &value = DFileHelper::attributeFromInfo(AttributeID::UnixMode, fileInfo);
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

bool DFile::setPermissions(Permissions permission)
{
    uint32_t stMode = 0000;
    if (permission.testFlag(Permission::ExeOwner) | permission.testFlag(Permission::ExeUser))
        stMode |= S_IXUSR;
    if (permission.testFlag(Permission::WriteOwner) | permission.testFlag(Permission::WriteUser))
        stMode |= S_IWUSR;
    if (permission.testFlag(Permission::ReadOwner) | permission.testFlag(Permission::ReadUser))
        stMode |= S_IRUSR;

    if (permission.testFlag(Permission::ExeGroup))
        stMode |= S_IXGRP;
    if (permission.testFlag(Permission::WriteGroup))
        stMode |= S_IWGRP;
    if (permission.testFlag(Permission::ReadGroup))
        stMode |= S_IRGRP;

    if (permission.testFlag(Permission::ExeOther))
        stMode |= S_IXOTH;
    if (permission.testFlag(Permission::WriteOther))
        stMode |= S_IWOTH;
    if (permission.testFlag(Permission::ReadOther))
        stMode |= S_IROTH;

    g_autoptr(GFile) gfile = DFileHelper::fileNewForUrl(d->url);

    g_autoptr(GCancellable) cancellable = g_cancellable_new();
    g_autoptr(GError) gerror = nullptr;
    const std::string &attributeKey = DFileHelper::attributeKey(AttributeID::UnixMode);
    bool succ = DFileHelper::setAttribute(gfile, attributeKey.c_str(), AttributeType::TypeUInt32, stMode, G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, cancellable, &gerror);
    if (gerror)
        d->setError(IOErrorCode(gerror->code));
    return succ;
}

bool DFile::setAttribute(AttributeID id, const QVariant &value)
{
    g_autoptr(GFile) gfile = DFileHelper::fileNewForUrl(d->url);
    const std::string &key = DFileHelper::attributeKey(id);
    const AttributeType &type = DFileHelper::attributeType(id);

    g_autoptr(GCancellable) cancellabel = g_cancellable_new();
    g_autoptr(GError) gerror = nullptr;

    if (key.empty() || type == AttributeType::TypeInvalid) {
        d->setError(IOErrorCode::InvalidArgument);
        return false;
    }

    bool succ = DFileHelper::setAttribute(gfile, key.c_str(), type, value, G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, cancellabel, &gerror);
    return succ;
}

bool DFile::setAttribute(const QByteArray &key, const QVariant &value, const AttributeType type, const FileQueryInfoFlags flag)
{
    g_autoptr(GFile) gfile = DFileHelper::fileNewForUrl(d->url);
    g_autoptr(GCancellable) cancellabel = g_cancellable_new();
    g_autoptr(GError) gerror = nullptr;

    bool succ = DFileHelper::setAttribute(gfile, key, type, value, GFileQueryInfoFlags(flag), cancellabel, &gerror);
    return succ;
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

DError DFile::lastError() const
{
    return d->error;
}

DIO_END_NAMESPACE
