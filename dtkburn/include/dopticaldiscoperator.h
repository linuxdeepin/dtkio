// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DOPTICALDISCOPERATOR_H
#define DOPTICALDISCOPERATOR_H

#include <QObject>

#include <DExpected>

#include "dtkburn_global.h"
#include "dtkburntypes.h"

DBURN_BEGIN_NAMESPACE

class DOpticalDiscOperatorPrivate;
class DOpticalDiscOperator final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DOpticalDiscOperator)

public:
    ~DOpticalDiscOperator() override;

    void setSpeed(int speed);
    void setVolumeName(const QString &name);
    void setCheckPrecision(double precision);

    DTK_CORE_NAMESPACE::DExpected<bool> burn(const QString &stagePath, const BurnOptions &opts);
    DTK_CORE_NAMESPACE::DExpected<bool> erase();
    DTK_CORE_NAMESPACE::DExpected<bool> checkmedia();
    DTK_CORE_NAMESPACE::DExpected<bool> writeISO(const QString &isoPath);
    DTK_CORE_NAMESPACE::DExpected<bool> dumpISO(const QString &isoPath);

Q_SIGNALS:
    void jobStatusChanged(JobStatus status, int progress, QString speed, QStringList message);

private:
    explicit DOpticalDiscOperator(QObject *parent = nullptr);

private:
    // TODO(zhangs): QScopedPointer<DOpticalDiscManagerPrivate> dptr;
};

DBURN_END_NAMESPACE

#endif   // DOPTICALDISCOPERATOR_H
