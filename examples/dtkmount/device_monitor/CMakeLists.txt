cmake_minimum_required(VERSION 3.5)

project(device_monitor VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

find_package(Qt5 COMPONENTS Widgets Core Gui DBus LinguistTools REQUIRED)
find_package(Dtk COMPONENTS Widget Core Gui REQUIRED)
find_package(dtkmount REQUIRED)

file(GLOB TS_FILES CONFIGURE_DEPENDS translations/*.ts)
file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS src/*.cpp include/*.h ${TS_FILES})

qt5_create_translation(
    QM_FILES 
    ${PROJECT_SOURCES} 
    ${TS_FILES}
)

add_executable(
    ${PROJECT_NAME}
    ${QM_FILES} 
    ${PROJECT_SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    ${DtkWidget_LIBRARIES}
    ${DtkCore_LIBRARIES}
    ${dtkmount_LIBRARIES}
    Qt5::Widgets
    Qt5::Core
    Qt5::DBus
)
