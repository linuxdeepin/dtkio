// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef UTILS_H
#define UTILS_H

#include <gio/gio.h>

#include <QString>
#include <QPair>

namespace Utils {

QString workingUser();

QString mountPointOf(GMount *mount);
QPair<QString, QString> mountInfoOf(GMount *mount);

}   // namespace Utils

#endif   // UTILS_H
