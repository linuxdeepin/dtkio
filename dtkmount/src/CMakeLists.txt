# SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: LGPL-3.0-or-later


file(GLOB_RECURSE INCLUDE_FILES "../include/*.h")
file(GLOB_RECURSE SRCS
    "./*.h"
    "./*.cpp"
)

# using classes generated from D-Bus XML
set(UDISKS2_OBJECT_XML ../misc/org.freedesktop.UDisks2.ObjectManager.xml)
set(UDISKS2_XML ../misc/org.freedesktop.UDisks2.xml)
set_source_files_properties(
    ${UDISKS2_XML}
    PROPERTIES NO_NAMESPACE true INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/dbus/udisks2_dbus_common.h
    )
qt5_add_dbus_interface(SRCS ${UDISKS2_OBJECT_XML} objectmanager_interface)
qt5_add_dbus_interface(SRCS ${UDISKS2_XML} udisks2_interface)

add_library(${BIN_NAME} SHARED
    ${INCLUDE_FILES}
    ${SRCS}
)

include_directories(../include)

set_target_properties(${BIN_NAME} PROPERTIES
    VERSION ${CMAKE_PROJECT_VERSION}
    SOVERSION ${CMAKE_PROJECT_VERSION_MAJOR})

target_compile_definitions(${BIN_NAME} PRIVATE VERSION="${CMAKE_PROJECT_VERSION}")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(GIOUNIX REQUIRED gio-unix-2.0)
pkg_check_modules(LIBSECRET REQUIRED libsecret-1)

target_include_directories(${BIN_NAME} PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::DBus
    ${GLIB_INCLUDE_DIRS}
    ${GIOUNIX_INCLUDE_DIRS}
    ${LIBSECRET_INCLUDE_DIRS}
    ${DtkCore_INCLUDE_DIRS}
)

target_link_libraries(${BIN_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::DBus
    ${GLIB_LIBRARIES}
    ${GIOUNIX_LIBRARIES}
    ${LIBSECRET_LIBRARIES}
    ${DtkCore_LIBRARIES}
)

install(FILES ${INCLUDE_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${BIN_NAME})
install(TARGETS ${BIN_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
