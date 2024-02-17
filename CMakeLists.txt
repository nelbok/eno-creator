cmake_minimum_required(VERSION 3.27)

include(cmake/config.cmake)

project(${CENO_SUITE} VERSION ${ENO_VERSION} LANGUAGES CXX)
configure_file(Config.hpp.in include/Config.hpp)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else()
    # additional warnings
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Quick Quick3D 3DExtras 3DCore 3DRender)
find_package(Qt6 REQUIRED COMPONENTS Widgets Quick Quick3D 3DExtras 3DCore 3DRender)

include(cmake/deployqt.cmake)

add_subdirectory(eno)
add_subdirectory(creator)
add_subdirectory(viewer)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CENO_CREATOR})
