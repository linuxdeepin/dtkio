// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DOPTICALDISCINFO_H
#define DOPTICALDISCINFO_H

#include <QObject>
#include <QString>
#include <QSharedDataPointer>

#include "dtkburn_global.h"
#include "dtkburntypes.h"

DBURN_BEGIN_NAMESPACE

class DOpticalDiscInfoPrivate;
class DOpticalDiscInfo final
{
public:
    DOpticalDiscInfo(const DOpticalDiscInfo &info);
    DOpticalDiscInfo &operator=(const DOpticalDiscInfo &info);
    ~DOpticalDiscInfo();

    bool blank() const;
    QString device() const;
    QString volumeName() const;
    quint64 usedSize() const;
    quint64 availableSize() const;
    quint64 totalSize() const;
    quint64 dataBlocks() const;
    MediaType mediaType() const;
    QStringList writeSpeed() const;

private:
    DOpticalDiscInfo();
    DOpticalDiscInfo(const QString &dev);

protected:
    // TODO(zhangs): QSharedDataPointer<DOpticalDiscInfoPrivate> d_ptr;
};

DBURN_END_NAMESPACE

#endif   // DOPTICALDISCINFO_H
