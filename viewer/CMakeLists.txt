set(PROJECT_SOURCES
	src/main.cpp

	resources/resource.h
	resources/logo.ico
	resources/resources.rc.in
	resources/resources.qrc
)
set(QML_SOURCES
	qml/Main.qml
)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${PROJECT_SOURCES} ${QML_SOURCES})
include_directories(.)
include_directories(src)

configure_file(resources/resources.rc.in resources/resources.rc)
configure_file(resources/resource.h resources/resource.h COPYONLY)
configure_file(resources/logo.ico resources/logo.ico COPYONLY)
set(OTHER_FILES
	${CMAKE_CURRENT_BINARY_DIR}/resources/resources.rc
)

qt_add_executable(${CENO_VIEWER} MANUAL_FINALIZATION ${PROJECT_SOURCES} ${OTHER_FILES})
qt_add_qml_module(${CENO_VIEWER} URI Viewer VERSION 1.0 RESOURCE_PREFIX / QML_FILES ${QML_SOURCES})

target_link_libraries(${CENO_VIEWER} PRIVATE
	Qt6::Quick
	${CENO_ENO}
)

set_target_properties(${CENO_VIEWER} PROPERTIES
	MACOSX_BUNDLE_GUI_IDENTIFIER ${CENO_VIEWER}.${CENO_ORGANIZATION}.com
	MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
	MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
	MACOSX_BUNDLE TRUE
	WIN32_EXECUTABLE TRUE
)

qt_finalize_executable(${CENO_VIEWER})

if(WIN32)
	windeployqt(${CENO_VIEWER} QMLDIR "${CMAKE_CURRENT_SOURCE_DIR}/qml")
endif()