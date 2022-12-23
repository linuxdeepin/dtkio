// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QStandardPaths>
#include <QDir>
#include <QMap>

#include "dsearch_p.h"
#include "dsearch.h"

DSEARCH_BEGIN_NAMESPACE

DSearchPrivate::DSearchPrivate(quint32 maxCount, SearchFlags flags)
    : maxResultCount(maxCount),
      searchFlags(flags)
{
}

DSearch::DSearch(QObject *parent)
    : QObject(parent),
      d_ptr(new DSearchPrivate())
{
}

DSearch::DSearch(quint32 maxResultCount, SearchFlags flags, QObject *parent)
    : QObject(parent),
      d_ptr(new DSearchPrivate(maxResultCount, flags))
{
}

DSearch::~DSearch()
{
}

bool DSearch::search(const QString &path, const QString &keyword)
{
    return true;
}

bool DSearch::stop()
{
    emit stoped();
    return true;
}

QStringList DSearch::matchedResults() const
{
    return {};
}

void DSearch::setResultFilter(const DSearch::ResultFilterFunc &filter)
{
    Q_D(DSearch);

    d->resultFilter = filter;
}

void DSearch::setMaxResultCount(quint32 maxResultCount)
{
    Q_D(DSearch);

    d->maxResultCount = maxResultCount;
}

quint32 DSearch::maxResultCount() const
{
    Q_D(const DSearch);

    return d->maxResultCount;
}

void DSearch::setSearchFlags(SearchFlags flags)
{
    Q_D(DSearch);

    d->searchFlags = flags;
}

SearchFlags DSearch::searchFlags() const
{
    Q_D(const DSearch);

    return d->searchFlags;
}

void DSearch::setIndexPath(const QString &path)
{
    Q_D(DSearch);

    d->indexPath = path;
}

QString DSearch::indexPath() const
{
    Q_D(const DSearch);

    return d->indexPath;
}

void DSearch::setAutoIndex(bool index)
{
    Q_D(DSearch);

    d->autoIndex = index;
}

bool DSearch::createIndex(const QString &path, SearchFlags flags)
{
    return true;
}

bool DSearch::updateIndex(const QString &path, SearchFlags flags)
{
    return true;
}

bool DSearch::clearIndex(SearchFlags flags)
{
    return true;
}

QMap<SearchFlag, IndexState> DSearch::indexState(SearchFlags flags)
{
    return {};
}

DSEARCH_END_NAMESPACE
