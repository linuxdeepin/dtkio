// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DTKSEARCHTYPES_H
#define DTKSEARCHTYPES_H

#include <QFlags>

#include "dtksearch_global.h"

DSEARCH_BEGIN_NAMESPACE

//!<@~english The search flags
enum class SearchFlag : uint8_t {
    FileName = 0x01,    //!<@~english Search for the filename.
    PinYin = 0x02 | FileName,   //!<@~english Search for the pinyin of the filename, and also recall the filename results that match the search keyword.
    FullText = 0x04,    //!<@~english Search for the file contents.
    AllEntries = PinYin | FullText  //!<@~english Search for filename, pinyin of filename and file contents.
};
Q_DECLARE_FLAGS(SearchFlags, SearchFlag)

//!<@~english The index states
enum IndexState : uint8_t {
    NotExists,  //!<@~english The index files are not exists.
    Creating,   //!<@~english The index files are being creating.
    Completed   //!<@~english The index files has been created.
};

DSEARCH_END_NAMESPACE

#endif // DTKSEARCHTYPES_H
