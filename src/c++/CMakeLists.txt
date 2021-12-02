set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")

set(SOURCE_DIR "${ROOT_DIR}/src/c++")
include("${ROOT_DIR}/cmake/sourcelist.cmake")

source_group(TREE "${SOURCE_DIR}" PREFIX "src" FILES ${SOURCE_FILES})

set(VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/include/Plugin.h")
configure_file(
	"${ROOT_DIR}/cmake/Plugin.h.in"
	"${VERSION_HEADER}"
	@ONLY
)

source_group("include" FILES "${VERSION_HEADER}")

add_library(
	"${PROJECT_NAME}"
	SHARED
	${SOURCE_FILES}
	"${VERSION_HEADER}"
	"${ROOT_DIR}/.clang-format"
	"${ROOT_DIR}/.editorconfig"
	"${ROOT_DIR}/QuickLootRE.toml"
)

target_compile_features(
	"${PROJECT_NAME}"
	PRIVATE
		cxx_std_20
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	target_compile_options(
		"${PROJECT_NAME}"
		PRIVATE
			"/sdl"	# Enable Additional Security Checks
			"/utf-8"	# Set Source and Executable character sets to UTF-8
			"/Zi"	# Debug Information Format

			"/permissive-"	# Standards conformance
			"/Zc:preprocessor"	# Enable preprocessor conformance mode

			"/wd4200" # nonstandard extension used : zero-sized array in struct/union

			"$<$<CONFIG:DEBUG>:>"
			"$<$<CONFIG:RELEASE>:/Zc:inline;/JMC-;/Ob3>"
	)

	target_link_options(
		"${PROJECT_NAME}"
		PRIVATE
			"$<$<CONFIG:DEBUG>:/INCREMENTAL;/OPT:NOREF;/OPT:NOICF>"
			"$<$<CONFIG:RELEASE>:/INCREMENTAL:NO;/OPT:REF;/OPT:ICF;/DEBUG:FULL>"
	)
endif()

target_include_directories(
	"${PROJECT_NAME}"
	PRIVATE
		"${CMAKE_CURRENT_BINARY_DIR}/include"
		"${INCLUDE_DIR}"
		"${SOURCE_DIR}"
)

set(SKSE_SUPPORT_XBYAK ON CACHE BOOL "")

add_subdirectory("${ROOT_DIR}/external/CommonLibSSE" CommonLibSSE)

find_package(AutoTOML REQUIRED CONFIG)
find_package(Boost
	MODULE
	REQUIRED
	COMPONENTS
		regex
)
find_package(frozen REQUIRED CONFIG)
find_package(spdlog REQUIRED CONFIG)
find_package(xbyak REQUIRED CONFIG)

target_link_libraries(
	"${PROJECT_NAME}"
	PRIVATE
		AutoTOML::AutoTOML
		Boost::regex
		CommonLibSSE::CommonLibSSE
		frozen::frozen
		spdlog::spdlog
		xbyak::xbyak
)

target_precompile_headers(
	"${PROJECT_NAME}"
	PRIVATE
		"${SOURCE_DIR}/PCH.h"
)

if("${COPY_OUTPUT}")
	add_custom_command(
		TARGET "${PROJECT_NAME}"
		POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${Skyrim64Path}/Data/SKSE/Plugins/"
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${ROOT_DIR}/QuickLootRE.toml" "${Skyrim64Path}/Data/SKSE/Plugins/"
		VERBATIM
	)
endif()
