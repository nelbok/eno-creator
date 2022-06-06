cmake_minimum_required(VERSION 3.5)

set(ENO_NAME "Eno Creator")
set(ENO_VERSION "1.0")

configure_file(src/Config.hpp.in src/Config.hpp)

project(EnoCreator VERSION ${ENO_VERSION} LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets 3DExtras 3DCore 3DRender)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets 3DExtras 3DCore 3DRender)

include(cmake/deployqt.cmake)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src)
include_directories(src)

set(PROJECT_SOURCES
	src/controller/MapAction.cpp
	src/controller/MapAction.hpp
	src/controller/Shortcuts.cpp
	src/controller/Shortcuts.hpp

	src/data/Material.cpp
	src/data/Material.hpp
	src/data/Materials.cpp
	src/data/Materials.hpp
	src/data/Project.cpp
	src/data/Project.hpp
	src/data/Scene.cpp
	src/data/Scene.hpp

	src/engine/Engine.cpp
	src/engine/Engine.hpp
	
	src/io/Eno.cpp
	src/io/Eno.hpp
	src/io/WavefrontOBJ.cpp
	src/io/WavefrontOBJ.hpp

	src/view/GraphicsShape.cpp
	src/view/GraphicsShape.hpp
	src/view/GraphicsView.cpp
	src/view/GraphicsView.hpp
	src/view/MainWindow.cpp
	src/view/MainWindow.hpp

	src/widgets/ComboBoxWidget.cpp
	src/widgets/ComboBoxWidget.hpp
	src/widgets/InfoWidget.cpp
	src/widgets/InfoWidget.hpp
	src/widgets/MaterialsDockWidget.cpp
	src/widgets/MaterialsDockWidget.hpp
	src/widgets/SpinBoxWidget.cpp
	src/widgets/SpinBoxWidget.hpp

	src/Config.hpp.in
	${CMAKE_CURRENT_BINARY_DIR}/src/Config.hpp
	src/Utils.hpp

	src/main.cpp

	data/data.qrc

	LICENSE
	README.md
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${PROJECT_SOURCES})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
	qt_add_executable(EnoCreator MANUAL_FINALIZATION ${PROJECT_SOURCES})
else()
	add_executable(EnoCreator ${PROJECT_SOURCES})
endif()

target_link_libraries(EnoCreator PRIVATE
	Qt${QT_VERSION_MAJOR}::Widgets
	Qt${QT_VERSION_MAJOR}::3DCore
	Qt${QT_VERSION_MAJOR}::3DExtras
	Qt${QT_VERSION_MAJOR}::3DRender
)

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

if(WIN32)
	windeployqt(EnoCreator ${INSTALL_BIN_PATH})
endif()
