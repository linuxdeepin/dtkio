// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DUDISKSJOB_P_H
#define DUDISKSJOB_P_H

#include "dudisksjob.h"
#include "udisks2_interface.h"

DMOUNT_BEGIN_NAMESPACE

class DUDisksJobPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(DUDisksJob)

public:
    explicit DUDisksJobPrivate(DUDisksJob *qq)
        : QObject { qq }, q_ptr { qq } { }
    ~DUDisksJobPrivate() override { }

private:
    DUDisksJob *q_ptr { nullptr };

    OrgFreedesktopUDisks2JobInterface *iface { nullptr };
};

DMOUNT_END_NAMESPACE

#endif   // DUDISKSJOB_P_H
