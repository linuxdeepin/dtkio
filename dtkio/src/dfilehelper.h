// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFILEHELPER_H
#define DFILEHELPER_H

#include <map>

#include <QVariant>

#include <gio/gio.h>

#include "dtkio_global.h"
#include "dtkiotypes.h"

DIO_BEGIN_NAMESPACE

namespace DFileHelper {

static std::map<AttributeID, AttributeDetails> attributeDetailsMap();

std::string attributeKey(AttributeID id);
QVariant attributeDefaultValue(AttributeID id);
AttributeType attributeType(AttributeID id);
QVariant attributeFromInfo(GFileInfo *fileinfo, AttributeID id);
bool setAttribute(GFile *gfile, const char *key, AttributeType type, const QVariant &value, GFileQueryInfoFlags flags, GCancellable *cancellable, GError **error);

};   // namespace DFileHelper

DIO_END_NAMESPACE
#endif   // DFILEHELPER_H
