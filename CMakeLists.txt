cmake_minimum_required(VERSION 3.27)

include(eno/cmake/defines.cmake)
include(cmake/defines.cmake)

project(${CENO_SUITE} VERSION ${ENO_VERSION} LANGUAGES CXX)

include(eno/cmake/config.cmake)

configure_file(Config.hpp.in include/Config.hpp)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Quick Quick3D QuickWidgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets Quick Quick3D QuickWidgets)

include(cmake/deployqt.cmake)

add_subdirectory(eno)
add_subdirectory(engine)
add_subdirectory(creator)
add_subdirectory(viewer)

if (MSVC)
	include(cmake/visualstudio.cmake)
endif()
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CENO_CREATOR})
