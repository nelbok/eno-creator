set(PROJECT_SOURCES
	src/About.cpp
	src/About.hpp
	src/Intro.cpp
	src/Intro.hpp
	src/main.cpp

	resources/resource.h
	resources/logo.ico
	resources/resources.rc.in
	resources/resources.qrc
)
set(QML_SOURCES
	qml/About.qml
	qml/Loader.qml
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
qt_add_qml_module(${CENO_VIEWER} URI viewer VERSION 1.0 RESOURCE_PREFIX / QML_FILES ${QML_SOURCES})

target_link_libraries(${CENO_VIEWER} PRIVATE
	Qt6::Quick
	Qt6::Quick3D
	${CENO_ENO}
	${CENO_ENGINE}
	${CENO_ENGINE}plugin
)

configure_file(android/AndroidManifest.xml.in android/AndroidManifest.xml)
configure_file(android/res/drawable-hdpi/icon.png android/res/drawable-hdpi/icon.png COPYONLY)
configure_file(android/res/drawable-ldpi/icon.png android/res/drawable-ldpi/icon.png COPYONLY)
configure_file(android/res/drawable-mdpi/icon.png android/res/drawable-mdpi/icon.png COPYONLY)
configure_file(android/res/drawable-xhdpi/icon.png android/res/drawable-xhdpi/icon.png COPYONLY)
configure_file(android/res/drawable-xxhdpi/icon.png android/res/drawable-xxhdpi/icon.png COPYONLY)
configure_file(android/res/drawable-xxxhdpi/icon.png android/res/drawable-xxxhdpi/icon.png COPYONLY)

set_target_properties(${CENO_VIEWER} PROPERTIES
	MACOSX_BUNDLE_GUI_IDENTIFIER ${CENO_VIEWER}.${CENO_ORGANIZATION}.com
	MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
	MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
	MACOSX_BUNDLE TRUE
	WIN32_EXECUTABLE TRUE
	QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/android
)

qt_finalize_executable(${CENO_VIEWER})

if(WIN32)
	windeployqt(${CENO_VIEWER} QMLDIR "${CMAKE_CURRENT_SOURCE_DIR}/qml")
endif()