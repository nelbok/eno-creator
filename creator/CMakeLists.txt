set(PROJECT_SOURCES
	src/controller/MapAction.cpp
	src/controller/MapAction.hpp
	src/controller/Shortcuts.cpp
	src/controller/Shortcuts.hpp

	src/engine/CuboidGeometry.cpp
	src/engine/CuboidGeometry.hpp
	src/engine/CuboidMesh.cpp
	src/engine/CuboidMesh.hpp
	src/engine/Engine.cpp
	src/engine/Engine.hpp

	src/io/WavefrontOBJ.cpp
	src/io/WavefrontOBJ.hpp

	src/view/GraphicsShape.cpp
	src/view/GraphicsShape.hpp
	src/view/GraphicsView.cpp
	src/view/GraphicsView.hpp
	src/view/MainWindow.cpp
	src/view/MainWindow.hpp
	src/view/PreferencesWindow.cpp
	src/view/PreferencesWindow.hpp

	src/widgets/common/ColorButton.cpp
	src/widgets/common/ColorButton.hpp
	src/widgets/common/KeyButton.cpp
	src/widgets/common/KeyButton.hpp
	src/widgets/common/SpinBox.cpp
	src/widgets/common/SpinBox.hpp
	src/widgets/tools/ComboBoxTool.cpp
	src/widgets/tools/ComboBoxTool.hpp
	src/widgets/tools/SpinBoxTool.cpp
	src/widgets/tools/SpinBoxTool.hpp
	src/widgets/InfoWidget.cpp
	src/widgets/InfoWidget.hpp
	src/widgets/LocationWidget.cpp
	src/widgets/LocationWidget.hpp
	src/widgets/MaterialsDockWidget.cpp
	src/widgets/MaterialsDockWidget.hpp

	src/main.cpp

	resources/resource.h
	resources/logo.ico
	resources/resources.rc.in
	resources/resources.qrc
)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${PROJECT_SOURCES})
include_directories(.)
include_directories(src)

configure_file(resources/resources.rc.in resources/resources.rc)
configure_file(resources/resource.h resources/resource.h COPYONLY)
configure_file(resources/logo.ico resources/logo.ico COPYONLY)
set(OTHER_FILES
	${CMAKE_CURRENT_BINARY_DIR}/resources/resources.rc
)

qt_add_executable(${CENO_CREATOR} MANUAL_FINALIZATION ${PROJECT_SOURCES} ${OTHER_FILES})

target_link_libraries(${CENO_CREATOR} PRIVATE
	Qt${QT_VERSION_MAJOR}::Widgets
	Qt${QT_VERSION_MAJOR}::3DCore
	Qt${QT_VERSION_MAJOR}::3DExtras
	Qt${QT_VERSION_MAJOR}::3DRender
	${CENO_ENO}
)

set_target_properties(${CENO_CREATOR} PROPERTIES
	MACOSX_BUNDLE_GUI_IDENTIFIER ${CENO_CREATOR}.${CENO_ORGANIZATION}.com
	MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
	MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
	MACOSX_BUNDLE TRUE
	WIN32_EXECUTABLE TRUE
)

qt_finalize_executable(${CENO_CREATOR})

if(WIN32)
	windeployqt(${CENO_CREATOR})
endif()