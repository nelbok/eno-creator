set(CMAKE_DEBUG_POSTFIX _d)
set(PROJECT_SOURCES
	include/eno/data/Material.hpp
	include/eno/data/Object.hpp
	include/eno/data/Project.hpp
	include/eno/data/Scene.hpp

	include/eno/io/Eno.hpp
	include/eno/io/IOThread.hpp

	include/eno/tools/Merger.hpp
	include/eno/tools/Utils.hpp

	src/data/Material.cpp
	src/data/Object.cpp
	src/data/Project.cpp
	src/data/Scene.cpp

	src/io/Eno.cpp
	src/io/IOThread.cpp

	src/tools/Merger.cpp
	src/tools/Utils.cpp
)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${PROJECT_SOURCES})
include_directories(.)

qt_add_library(${CENO_ENO} STATIC ${PROJECT_SOURCES})

target_include_directories(${CENO_ENO} PUBLIC include)
target_link_libraries(${CENO_ENO} PRIVATE
	Qt6::Core
	Qt6::Gui
)
