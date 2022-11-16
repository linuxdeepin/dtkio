# SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: LGPL-3.0-or-later

set(BIN_NAME ut-dtkcompressor)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-access-control -fno-inline -Wno-pmf-conversions")

file(GLOB_RECURSE INCLUDE_FILES "../../include/*.h")
file(GLOB_RECURSE SRCS
    "../../src/*.h"
    "../../src/*.cpp"
    "../../3rdparty/libzipplugin/*.cpp"
)
file(GLOB_RECURSE TEST_FILES "./*.h" "./*.cpp")

include_directories(
    "../../include"
)
add_definitions(-DUSE_FAKE_INTERFACE)
add_executable(${BIN_NAME}
    ${INCLUDE_FILES}
    ${SRCS}
    ${TEST_FILES}
    ${DFAKE_INTERFACE_DBUS_XML}
)

find_package(PkgConfig REQUIRED)
find_package(DtkCore REQUIRED)
find_package(KF5Codecs REQUIRED)
find_package(ZLIB REQUIRED)

target_include_directories(${BIN_NAME} PUBLIC
    Qt${QT_VERSION_MAJOR}::DBus
    ${HFILES_OUTPUT_PATH}
    ../../3rdparty/libzipplugin
    ../../src/archivemanager
)

target_link_libraries(${BIN_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::DBus
    ${DtkCore_LIBRARIES}
    KF5::Codecs
    zip
    interfacecompressor
    -lpthread
    -lgcov
    -lgtest
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${BIN_NAME} PRIVATE -fprofile-instr-generate -ftest-coverage)
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${BIN_NAME} PRIVATE -fprofile-arcs -ftest-coverage)
endif()

enable_testing()
add_test(NAME ${BIN_NAME} COMMAND ${BIN_NAME})
