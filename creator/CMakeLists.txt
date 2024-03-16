set(PROJECT_SOURCES
	src/controller/command/BaseCommand.cpp
	src/controller/command/BaseCommand.hpp
	src/controller/command/ContainerCommand.hpp
	src/controller/command/CreateCommand.hpp
	src/controller/command/DestroyCommand.hpp
	src/controller/command/MaterialCommand.cpp
	src/controller/command/MaterialCommand.hpp
	src/controller/command/ObjectCommand.cpp
	src/controller/command/ObjectCommand.hpp
	src/controller/command/ProjectCommand.cpp
	src/controller/command/ProjectCommand.hpp
	src/controller/command/SceneCommand.cpp
	src/controller/command/SceneCommand.hpp
	src/controller/command/TextureCommand.cpp
	src/controller/command/TextureCommand.hpp
	src/controller/command/ValueCommand.hpp

	src/controller/Commands.cpp
	src/controller/Commands.hpp
	src/controller/Core.cpp
	src/controller/Core.hpp
	src/controller/Graphics.cpp
	src/controller/Graphics.hpp
	src/controller/Preferences.cpp
	src/controller/Preferences.hpp
	src/controller/RecentFiles.cpp
	src/controller/RecentFiles.hpp
	src/controller/Shortcuts.cpp
	src/controller/Shortcuts.hpp

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
	src/widgets/common/PixmapButton.cpp
	src/widgets/common/PixmapButton.hpp
	src/widgets/common/SpinBox.cpp
	src/widgets/common/SpinBox.hpp
	src/widgets/docks/BaseDockWidget.cpp
	src/widgets/docks/BaseDockWidget.hpp
	src/widgets/docks/MaterialsDockWidget.cpp
	src/widgets/docks/MaterialsDockWidget.hpp
	src/widgets/docks/ProjectDockWidget.cpp
	src/widgets/docks/ProjectDockWidget.hpp
	src/widgets/docks/TexturesDockWidget.cpp
	src/widgets/docks/TexturesDockWidget.hpp
	src/widgets/tools/ComboBoxTool.cpp
	src/widgets/tools/ComboBoxTool.hpp
	src/widgets/tools/SpinBoxTool.cpp
	src/widgets/tools/SpinBoxTool.hpp
	src/widgets/InfoWidget.cpp
	src/widgets/InfoWidget.hpp
	src/widgets/LocationWidget.cpp
	src/widgets/LocationWidget.hpp

	src/main.cpp

	resources/resource.h
	resources/logo.ico
	resources/resources.rc.in
	resources/resources.qrc
)
set(QML_SOURCES
	qml/Preview.qml
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
qt_add_qml_module(${CENO_CREATOR} URI creator VERSION 1.0 RESOURCE_PREFIX / QML_FILES ${QML_SOURCES})

target_link_libraries(${CENO_CREATOR} PRIVATE
	Qt6::Widgets
	Qt6::Quick
	Qt6::Quick3D
	Qt6::QuickWidgets
	${CENO_ENO}
	${CENO_ENGINE}
	${CENO_ENGINE}plugin
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
