// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DAUDIOOUT_P_H
#define DAUDIOOUT_P_H

#include "darchivemanager.h"
#include <QObject>

DCOMPRESSOR_BEGIN_NAMESPACE

class DArchiveManagerPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(DArchiveManager)

public:
    explicit DArchiveManagerPrivate(DArchiveManager *parent = nullptr)
        : QObject(parent), q_ptr(parent)
    {
    }
private Q_SLOTS:
    void slotJobFinished();

private:
    DArchiveManager *q_ptr;
    ArchiveJob *m_pArchiveJob = nullptr;
    ReadOnlyArchiveInterface *m_pInterface = nullptr;
    ReadOnlyArchiveInterface *m_pTempInterface = nullptr;
    bool m_bCancel = false;
    static QMutex m_mutex;
    static QAtomicPointer<DArchiveManager> m_instance;
};
DCOMPRESSOR_END_NAMESPACE
#endif
