cmake_minimum_required(VERSION 3.5)

project(EnoCreator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(src)

set(PROJECT_SOURCES
    src/creator/creator-action.hpp
    src/creator/creator.hpp
    src/creator/creator-item.hpp
    src/data/map.hpp
    src/data/misc.hpp
    src/files/creator-eno.hpp
    src/files/creator-wavefront.hpp
    src/help/help.hpp
    src/opengl/opengl.hpp
    src/creator-preference.hpp
    src/creator-settings.hpp
    src/creator-undoredo.hpp
    src/creator-window.hpp
    src/creator/creator-action.cpp
    src/creator/creator-item.cpp
    src/creator/creator.cpp
    src/data/map.cpp
    src/data/misc.cpp
    src/files/creator-eno.cpp
    src/files/creator-wavefront.cpp
    src/help/help.cpp
    src/opengl/opengl.cpp
    src/creator-preference.cpp
    src/creator-settings.cpp
    src/creator-undoredo.cpp
    src/creator-window.cpp
    src/main.cpp


    data/data.qrc

    src/creator-preference.ui
    src/creator-window.ui
    
    LICENSE
    README.md
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(EnoCreator
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
add_executable(EnoCreator
    ${PROJECT_SOURCES}
)
endif()

target_link_libraries(EnoCreator PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(EnoCreator PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER EnoCreator.SRP.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(EnoCreator)
endif()
