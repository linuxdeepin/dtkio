# SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: LGPL-3.0-or-later

cmake_minimum_required(VERSION 3.13)

# If do't define version number, specify the version number
set(VERSION "1.0.0" CACHE STRING "define project version")

project(dtkio
    LANGUAGES CXX
    HOMEPAGE_URL https://github.com/linuxdeepin/dtkio
    DESCRIPTION "dtk library for dtkio."
    VERSION ${VERSION})

add_subdirectory(dtkmount)
add_subdirectory(dtkcompressor)
add_subdirectory(dtkio)
add_subdirectory(dtksearch)
add_subdirectory(dtkburn)
