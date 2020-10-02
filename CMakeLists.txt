cmake_minimum_required(VERSION 3.18)

# ---- Project ----

project(
	QuickLootRE
	VERSION 2.8.1
	LANGUAGES CXX
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/Version.h.in
	${CMAKE_CURRENT_BINARY_DIR}/include/Version.h
	@ONLY
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
	${CMAKE_CURRENT_BINARY_DIR}/version.rc
	@ONLY
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(
		FATAL_ERROR
			"In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
)
endif()

# ---- Globals ----

set(MSVC_COMPILE_OPTIONS
	/Gy	# Enable Function-Level Linking
	/MP	# Build with Multiple Processes
	/Oi	# Generate Intrinsic Functions
	/sdl	# Enable Additional Security Checks
	/Zi	# Debug Information Format
	
	/permissive-	# Standards conformance
	
	/Zc:alignedNew	# C++17 over-aligned allocation
	/Zc:auto	# Deduce Variable Type
	/Zc:char8_t
	/Zc:__cplusplus	# Enable updated __cplusplus macro
	/Zc:externC
	/Zc:externConstexpr	# Enable extern constexpr variables
	/Zc:forScope	# Force Conformance in for Loop Scope
	/Zc:hiddenFriend
	/Zc:implicitNoexcept	# Implicit Exception Specifiers
	/Zc:lambda
	/Zc:noexceptTypes	# C++17 noexcept rules
	/Zc:preprocessor	# Enable preprocessor conformance mode
	/Zc:referenceBinding	# Enforce reference binding rules
	/Zc:rvalueCast	# Enforce type conversion rules
	/Zc:sizedDealloc	# Enable Global Sized Deallocation Functions
	/Zc:strictStrings	# Disable string literal type conversion
	/Zc:ternary	# Enforce conditional operator rules
	/Zc:threadSafeInit	# Thread-safe Local Static Initialization
	/Zc:tlsGuards
	/Zc:trigraphs	# Trigraphs Substitution
	/Zc:wchar_t	# wchar_t Is Native Type
	
	/experimental:external
	/external:anglebrackets
	/external:W0
	
	/W4	# Warning level (all warnings)
	/WX	# Warning level (warnings are errors)
)

set(MSVC_COMPILE_OPTIONS_DEBUG
	/Zc:inline-	# Keep unreferenced COMDAT
	/JMC	# Just My Code debugging
)

set(MSVC_COMPILE_OPTIONS_RELEASE
	/Zc:inline	# Remove unreferenced COMDAT
	/JMC-	# Disable Just My Code debugging
)

set(MSVC_LINK_OPTIONS_DEBUG
	/INCREMENTAL	# Link Incrementally
	/OPT:NOREF	# Optimizations (keep functions/data never referenced)
	/OPT:NOICF	# Optimizations (prevent identical COMDAT folding)
)

set(MSVC_LINK_OPTIONS_RELEASE
	/INCREMENTAL:NO	# Link Incrementally
	/OPT:REF	# Optimizations (eliminate functions/data never referenced)
	/OPT:ICF	# Optimizations (perform identical COMDAT folding)
)

add_compile_definitions(
	"$<$<BOOL:${MSVC}>:_UNICODE>"
	TOML_WINDOWS_COMPAT=0
	
	SKSE_CUSTOM_WINDEF
	
	NOGDICAPMASKS
	NOVIRTUALKEYCODES
	#NOWINMESSAGES
	NOWINSTYLES
	NOSYSMETRICS
	NOMENUS
	NOICONS
	NOKEYSTATES
	NOSYSCOMMANDS
	NORASTEROPS
	NOSHOWWINDOW
	OEMRESOURCE
	NOATOM
	NOCLIPBOARD
	NOCOLOR
	#NOCTLMGR
	NODRAWTEXT
	NOGDI
	NOKERNEL
	#NOUSER
	#NONLS
	#NOMB
	NOMEMMGR
	NOMETAFILE
	NOMINMAX
	#NOMSG
	NOOPENFILE
	NOSCROLL
	NOSERVICE
	NOSOUND
	NOTEXTMETRIC
	NOWH
	NOWINOFFSETS
	NOCOMM
	NOKANJI
	NOHELP
	NOPROFILER
	NODEFERWINDOWPOS
	NOMCX
)

add_compile_options(
	"$<$<BOOL:${MSVC}>:${MSVC_COMPILE_OPTIONS}>"
	"$<$<AND:$<BOOL:${MSVC}>,$<CONFIG:DEBUG>>:${MSVC_COMPILE_OPTIONS_DEBUG}>"
	"$<$<AND:$<BOOL:${MSVC}>,$<CONFIG:RELEASE>>:${MSVC_COMPILE_OPTIONS_RELEASE}>"
)

add_link_options(
	"$<$<AND:$<BOOL:${MSVC}>,$<CONFIG:DEBUG>>:${MSVC_LINK_OPTIONS_DEBUG}>"
	"$<$<AND:$<BOOL:${MSVC}>,$<CONFIG:RELEASE>>:${MSVC_LINK_OPTIONS_RELEASE}>"
)

# ---- Dependencies ----

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)

set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
set(Boost_USE_DEBUG_RUNTIME ON)

add_subdirectory($ENV{CommonLibSSEPath} CommonLibSSE)

find_package(Boost
	REQUIRED
	COMPONENTS
		regex
)
find_package(frozen REQUIRED CONFIG)
find_package(span-lite REQUIRED CONFIG)
find_package(spdlog REQUIRED CONFIG)
find_package(tomlplusplus REQUIRED CONFIG)

# ---- Add source files ----

include(cmake/headerlist.cmake)
include(cmake/sourcelist.cmake)

source_group(
	TREE ${CMAKE_CURRENT_SOURCE_DIR}
	FILES
		${headers}
		${sources}
)

source_group(
	TREE ${CMAKE_CURRENT_BINARY_DIR}
	FILES
		${CMAKE_CURRENT_BINARY_DIR}/include/Version.h
)

# ---- Create DLL ----

set(MSVC_TARGET_COMPILE_OPTIONS
	/TP	# Specify Source File Type (C++)
	
	# disable warnings
	/wd4324 # 'struct_name' : structure was padded due to __declspec(align())
)

add_library(QuickLootRE SHARED
	${headers}
	${sources}
	${CMAKE_CURRENT_BINARY_DIR}/include/Version.h
	${CMAKE_CURRENT_BINARY_DIR}/version.rc
	.clang-format
	QuickLootRE.toml
)

set_property(
	TARGET
		QuickLootRE
		CommonLibSSE
	PROPERTY
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

target_compile_features(QuickLootRE
	PUBLIC
		cxx_std_17
)

target_compile_options(QuickLootRE
	PRIVATE
		"$<$<BOOL:${MSVC}>:${MSVC_TARGET_COMPILE_OPTIONS}>"
)

target_include_directories(QuickLootRE
	PRIVATE 
		${CMAKE_CURRENT_BINARY_DIR}/include
		${CMAKE_CURRENT_SOURCE_DIR}/include
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(QuickLootRE
	PUBLIC
		Boost::regex
		CommonLibSSE
		frozen::frozen
		nonstd::span-lite
		spdlog::spdlog
		tomlplusplus::tomlplusplus
)

target_link_options(QuickLootRE
	PUBLIC
		"$<$<AND:$<BOOL:${MSVC}>,$<CONFIG:RELEASE>>:/DEBUG:FULL>"
)

target_precompile_headers(QuickLootRE
	PRIVATE
		src/PCH.h
)

# ---- Post build ----

add_custom_command(
	TARGET QuickLootRE
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:QuickLootRE> $ENV{Skyrim64Path}/Data/SKSE/Plugins/
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:QuickLootRE> $ENV{Skyrim64Path}/Data/SKSE/Plugins/
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/QuickLootRE.toml $ENV{Skyrim64Path}/Data/SKSE/Plugins/
)
