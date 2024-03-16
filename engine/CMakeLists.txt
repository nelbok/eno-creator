#set(CMAKE_DEBUG_POSTFIX _d)
set(PROJECT_SOURCES
	include/eno/engine/CuboidGeometry.hpp
	include/eno/engine/CuboidTexture.hpp

	src/engine/CuboidGeometry.cpp
	src/engine/CuboidTexture.cpp
)
set(QML_SOURCES
	qml/Material.qml
	qml/Viewer.qml
)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${PROJECT_SOURCES} ${QML_SOURCES})
include_directories(.)

qt_add_library(${CENO_ENGINE} STATIC ${PROJECT_SOURCES})
qt_add_qml_module(${CENO_ENGINE} URI engine VERSION 1.0 RESOURCE_PREFIX / QML_FILES ${QML_SOURCES})

target_include_directories(${CENO_ENGINE} PUBLIC include)
target_link_libraries(${CENO_ENGINE} PRIVATE
	Qt6::Quick
	Qt6::Quick3D
	${CENO_ENO}
)
