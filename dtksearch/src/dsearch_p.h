// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DSEARCH_P_H
#define DSEARCH_P_H

//#include "dtksearch_global.h"
#include "dsearch.h"

DSEARCH_BEGIN_NAMESPACE

class DSearchPrivate : public QObject
{
public:
    DSearchPrivate(quint32 maxCount = UINT32_MAX, SearchFlags flags = SearchFlag::FileName);

    quint32 maxResultCount;
    SearchFlags searchFlags;
    QString indexPath;
    DSearch::ResultFilterFunc resultFilter;
    bool autoIndex { false };
};

DSEARCH_END_NAMESPACE

#endif   // DSEARCH_P_H
