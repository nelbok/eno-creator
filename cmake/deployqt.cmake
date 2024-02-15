find_package(Qt${QT_VERSION_MAJOR}Core REQUIRED)

# Retrieve the absolute path to qmake and then use that path to find
# the windeployqt and macdeployqt binaries
get_target_property(_qmake_executable Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
if(WIN32 AND NOT WINDEPLOYQT_EXECUTABLE)
	message(FATAL_ERROR "windeployqt not found")
endif()

find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")
if(APPLE AND NOT MACDEPLOYQT_EXECUTABLE)
	message(FATAL_ERROR "macdeployqt not found")
endif()

# Add commands that copy the required Qt files to the same directory as the
# target after being built as well as including them in final installation
function(windeployqt target)
	cmake_parse_arguments(DEPLOY
		PARSED_ARGS # prefix of output variables
		"" # list of names of the boolean arguments (only defined ones will be true)
		"QMLDIR" # list of names of mono-valued arguments
		"" # list of names of multi-valued arguments (output variables are lists)
		${ARGN} # arguments of the function to parse, here we take the all original ones
	)
	if (DEPLOY_QMLDIR)
		set(ADD_QMLDIR --qmldir "${DEPLOY_QMLDIR}")
	endif()
	add_custom_command(TARGET ${target} POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E
		env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
		--verbose 0
		--no-compiler-runtime
		--no-opengl-sw
		--no-system-d3d-compiler
		--no-translations
		${ADD_QMLDIR}
		\"$<TARGET_FILE:${target}>\"
		COMMENT "Deploying Qt..."
	)
endfunction()

# Add commands that copy the required Qt files to the application bundle
# represented by the target.
function(macdeployqt target)
	cmake_parse_arguments(DEPLOY
		PARSED_ARGS # prefix of output variables
		"" # list of names of the boolean arguments (only defined ones will be true)
		"QMLDIR" # list of names of mono-valued arguments
		"" # list of names of multi-valued arguments (output variables are lists)
		${ARGN} # arguments of the function to parse, here we take the all original ones
	)
	# FIX ME
	#if (DEPLOY_QMLDIR)
	#	set(ADD_QMLDIR --qmldir "${DEPLOY_QMLDIR}")
	#endif()
	add_custom_command(TARGET ${target} POST_BUILD
		COMMAND "${MACDEPLOYQT_EXECUTABLE}"
		\"$<TARGET_FILE_DIR:${target}>/../..\"
		-always-overwrite
		COMMENT "Deploying Qt..."
	)
endfunction()

mark_as_advanced(WINDEPLOYQT_EXECUTABLE MACDEPLOYQT_EXECUTABLE)