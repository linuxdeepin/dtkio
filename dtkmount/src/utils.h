// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef UTILS_H
#define UTILS_H

#include <gio/gio.h>

#include <QString>
#include <QPair>

namespace Utils {
// daemon mounts mounted at "/media/$USER/smbmounts/hello on 1.2.3.4"
static constexpr char kRegxDaemonMountPath[] { "^/media/.*/smbmounts/" };
static constexpr char kDaemonService[] { "com.deepin.filemanager.daemon" };
static constexpr char kDaemonPath[] { "/com/deepin/filemanager/daemon" };
static constexpr char kDaemonIntro[] { "org.freedesktop.DBus.Introspectable" };
static constexpr char kDaemonIntroMethod[] { "Introspect" };
static constexpr char kMountControlPath[] { "/com/deepin/filemanager/daemon/MountControl" };
static constexpr char kMountControlIFace[] { "com.deepin.filemanager.daemon.MountControl" };
static constexpr char kMountControlMount[] { "Mount" };
static constexpr char kMountControlUnmount[] { "Unmount" };

QString workingUser();

QString mountPointOf(GMount *mount);
QPair<QString, QString> mountInfoOf(GMount *mount);

}   // namespace Utils

#endif   // UTILS_H
