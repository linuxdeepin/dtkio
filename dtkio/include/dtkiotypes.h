// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DTKIOTYPES_H
#define DTKIOTYPES_H

#include <stdint.h>

#include <QFlags>

#include "dtkio_global.h"

DIO_BEGIN_NAMESPACE

enum class OpenFlag : quint16 {
    NotOpen = 0x0000,
    ReadOnly = 0x0001,
    WriteOnly = 0x0002,
    ReadWrite = ReadOnly | WriteOnly,
    Append = 0x0004,
    Truncate = 0x0008,
    Text = 0x0010,
    Unbuffered = 0x0020,
    NewOnly = 0x0040,
    ExistingOnly = 0x0080,

    CustomStart = 0x0100
};
Q_DECLARE_FLAGS(OpenFlags, OpenFlag)

enum class CopyFlag : quint8 {
    None = 0,   // No flags set.
    Overwrite = 1,   // Overwrite any existing files.
    Backup = 2,   // Make a backup of any existing files.
    NoFollowSymlinks = 3,   // Don’t follow symlinks.
    AllMetadata = 4,   // Copy all file metadata instead of just default set used for copy.
    NoFallbackForMove = 5,   // Don’t use copy and delete fallback if native move not supported.
    TargetDefaultPerms = 6,   // Leaves target file with default perms, instead of setting the source file perms.

    UserFlag = 0x10
};

enum class AttributeID : quint16 {
    StandardType = 0,   // uint32
    StandardIsHidden = 1,   // boolean
    StandardIsBackup = 2,   // boolean
    StandardIsSymlink = 3,   // boolean
    StandardIsVirtual = 4,   // boolean
    StandardIsVolatile = 5,   // boolean
    StandardName = 6,   // byte string
    StandardDisplayName = 7,   // string
    StandardEditName = 8,   // string
    StandardCopyName = 9,   // string
    StandardIcon = 10,   // QList<QString>
    StandardSymbolicIcon = 11,   // QList<QString>
    StandardContentType = 12,   // string
    StandardFastContentType = 13,   // string
    StandardSize = 14,   // uint64
    StandardAllocatedSize = 15,   // uint64
    StandardSymlinkTarget = 16,   // byte string
    StandardTargetUri = 17,   // string
    StandardSortOrder = 18,   // int32
    StandardDescription = 19,   // string

    EtagValue = 40,   // string

    IdFile = 60,   // string
    IdFilesystem = 61,   // string

    AccessCanRead = 100,   // boolean
    AccessCanWrite = 101,   // boolean
    AccessCanExecute = 102,   // boolean
    AccessCanDelete = 103,   // boolean
    AccessCanTrash = 104,   // boolean
    AccessCanRename = 105,   // boolean

    MountableCanMount = 130,   // boolean
    MountableCanUnmount = 131,   // boolean
    MountableCanEject = 132,   // boolean
    MountableUnixDevice = 133,   // uint32
    MountableUnixDeviceFile = 134,   // string
    MountableHalUdi = 135,   // string
    MountableCanPoll = 136,   // boolean
    MountableIsMediaCheckAutomatic = 137,   // boolean
    MountableCanStart = 138,   // boolean
    MountableCanStartDegraded = 139,   // boolean
    MountableCanStop = 140,   // boolean
    MountableStartStopType = 141,   // uint32

    TimeModified = 200,   // uint64
    TimeModifiedUsec = 201,   // uint32
    TimeAccess = 202,   // uint64
    TimeAccessUsec = 203,   // uint32
    TimeChanged = 204,   // uint64
    TimeChangedUsec = 205,   // uint32
    TimeCreated = 206,   // uint64
    TimeCreatedUsec = 207,   // uint32

    UnixDevice = 330,   // uint32
    UnixInode = 331,   // uint64
    UnixMode = 332,   // uint32
    UnixNlink = 333,   // uint32
    UnixUID = 334,   // uint32
    UnixGID = 335,   // uint32
    UnixRdev = 336,   // uint32
    UnixBlockSize = 337,   // uint32
    UnixBlocks = 338,   // uint64
    UnixIsMountPoint = 339,   // boolean

    DosIsArchive = 360,   // boolean
    DosIsSystem = 361,   // boolean

    OwnerUser = 300,   // string
    OwnerUserReal = 301,   // string
    OwnerGroup = 302,   // string

    ThumbnailPath = 390,   // byte string
    ThumbnailFailed = 391,   // boolean
    ThumbnailIsValid = 392,   // boolean

    PreviewIcon = 420,   // object

    FileSystemSize = 440,   // uint64
    FileSystemFree = 441,   // uint64
    FileSystemUsed = 442,   // uint64
    FileSystemType = 443,   // string
    FileSystemReadOnly = 444,   // boolean
    FileSystemUsePreview = 445,   // uint32
    FileSystemRemote = 446,   // boolean

    GvfsBackend = 470,   // string

    SelinuxContext = 490,   // string

    TrashItemCount = 510,   // uint32
    TrashDeletionDate = 511,   // string
    TrashOrigPath = 512,   // byte string

    RecentModified = 540,   // uint64

    CustomStart = 600,

    StandardIsFile = 610,
    StandardIsDir = 611,
    StandardIsRoot = 612,
    StandardSuffix = 613,
    StandardCompleteSuffix = 614,
    StandardFilePath = 615,
    StandardParentPath = 616,
    StandardBaseName = 617,
    StandardFileName = 618,
    StandardCompleteBaseName = 619,

    AttributeIDMax = 999,
};

enum class SeekType : quint8 {
    Begin = 0x00,
    Current = 0x01,
    End = 0x02
};

enum class Permission : quint16 {
    NoPermission = 0x0000,

    ExeOther = 0x0001,
    WriteOther = 0x0002,
    ReadOther = 0x0004,

    ExeGroup = 0x0010,
    WriteGroup = 0x0020,
    ReadGroup = 0x0040,

    ExeUser = 0x0100,
    WriteUser = 0x0200,
    ReadUser = 0x0400,

    ExeOwner = 0x1000,
    WriteOwner = 0x2000,
    ReadOwner = 0x4000,
};
Q_DECLARE_FLAGS(Permissions, Permission)

enum class DFileAttributeType : quint8 {
    TypeInvalid = 0,   // Indicates an invalid or uninitialized type
    TypeString = 1,   // A null terminated UTF8 string
    TypeByteString = 2,   // A zero terminated string of non-zero bytes
    TypeBool = 3,   // A boolean value
    TypeUInt32 = 4,   // An unsigned 4-byte/32-bit integer
    TypeInt32 = 5,   // A signed 4-byte/32-bit integer
    TypeUInt64 = 6,   // An unsigned 8-byte/64-bit integer
    TypeInt64 = 7,   // A signed 8-byte/64-bit integer
    TypeObject = 8,   // A Object
    TypeStringV = 9   // A NULL terminated char **
};

enum class FileQueryInfoFlags : quint8 {
    TypeNone,
    TypeNoFollowSymlinks
};

enum class DirFilter : qint16 {
    NoFilter = 0x0000,   // no filter
    Dirs = 0x0001,   // List directories that match the filters.
    Files = 0x0002,   // List files.
    Drives = 0x0004,   // List disk drives (ignored under Unix).
    AllEntries = Dirs | Files | Drives,   // List directories, files, drives and symlinks (this does not list broken symlinks unless you specify System).
    NoSymLinks = 0x0008,   // Do not list symbolic links (ignored by operating systems that don't support symbolic links).

    Readable = 0x0010,   // List files for which the application has read access. The Readable value needs to be combined with Dirs or Files.
    Writable = 0x0020,   // List files for which the application has write access. The Writable value needs to be combined with Dirs or Files.
    Executable = 0x0040,   // List files for which the application has execute access. The Executable value needs to be combined with Dirs or Files.
    Modified = 0x0080,   // Only list files that have been modified (ignored on Unix).

    Hidden = 0x0100,   // List hidden files (on Unix, files starting with a ".").
    System = 0x0200,   // List system files (on Unix, FIFOs, sockets and device files are included; on Windows, .lnk files are included)
    AllDirs = 0x0400,   // List all directories; i.e. don't apply the filters to directory names.
    CaseSensitive = 0x0800,   // The filter should be case sensitive.

    NoDot = 0x2000,   // Do not list the special entry ".".
    NoDotDot = 0x4000,   // Do not list the special entry "..".
    NoDotAndDotDot = NoDot | NoDotDot,   // Do not list the special entries "." and "..".
};
Q_DECLARE_FLAGS(DirFilters, DirFilter)

enum class IteratorFlag : quint8 {
    NoIteratorFlags = 0x00,   // The default value, representing no flags. The iterator will return entries for the assigned path.
    FollowSymlinks = 0x01,   // When combined with Subdirectories, this flag enables iterating through all subdirectories of the assigned path, following all symbolic links. Symbolic link loops (e.g., "link" => "." or "link" => "..") are automatically detected and ignored.
    Subdirectories = 0x02,   // List entries inside all subdirectories as well.
};
Q_DECLARE_FLAGS(IteratorFlags, IteratorFlag)

enum class WatchType : quint8 {
    Auto = 0x00,
    Dir = 0x01,
    File = 0x02,
};

DIO_END_NAMESPACE

#endif   //DTKIOTYPES_H
