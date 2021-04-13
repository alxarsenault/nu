# This needs to be included before everything.
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.conan.txt)

cmake_minimum_required(VERSION 3.8.0)


#User options
# option(NU_GENRATE_DOC "Generate documentation." ON)

MACRO(add_sub_directories directory)
  file(GLOB FOLDERS ${directory})
  foreach(folder ${FOLDERS})
    if(EXISTS ${folder}/CMakeLists.txt)
      add_subdirectory(${folder})
    endif()
  endforeach()
ENDMACRO()

function(make_pretty_source_folders BASE_PATH PREFIX_PATH REQUIRED_ARG)
	foreach(SOURCE IN ITEMS ${REQUIRED_ARG})
		get_filename_component(SOURCE_PATH "${SOURCE}" PATH)
		# message("pretty source_path : ${SOURCE_PATH}")
		file(RELATIVE_PATH SOURCE_PATH_REL "${CMAKE_CURRENT_SOURCE_DIR}/${BASE_PATH}" "${SOURCE_PATH}")
		# message("pretty source_path_rel : ${SOURCE_PATH_REL}")
		string(REPLACE "/" "\\" GROUP_PATH "${PREFIX_PATH}/${SOURCE_PATH_REL}")
		# message("pretty group_path : ${GROUP_PATH}")
		source_group("${GROUP_PATH}" FILES "${SOURCE}")
	endforeach()
endfunction()

# Prevent from running cmake in root folder.
if(CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
  message(FATAL_ERROR "CMake needs to be called in a sub directory.")
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug"  CACHE STRING "Choose the type of build." FORCE)
endif()

set(CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE})

set(CMAKE_XCODE_GENERATE_SCHEME ON)

project(nu)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compile Options
function(nu_set_compile_options REQUIRED_ARG EXPOSURE)
  #-------------------------------------------------------------
  # Clang.
  #-------------------------------------------------------------
	if (("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") OR ("${CMAKE_CXX_COMPILER_ID}" MATCHES "AppleClang"))
    set(NU_CLANG_COMMON_FLAGS
      # -std=gnu++2a
      -stdlib=libc++
      -Wall
      -Wpedantic
      -Woverloaded-virtual
      -Wreorder
      -Wuninitialized
      -Wunused-parameter
      -Wshift-sign-overflow
      -Wno-switch
      -Wno-switch-enum
      -Wno-ignored-qualifiers
    )

    if (CMAKE_BUILD_TYPE EQUAL "Release")
      # Release.
      target_compile_definitions(${PROJECT_NAME} ${EXPOSURE}
        "NDEBUG=1"
      )

      target_compile_options(${REQUIRED_ARG} ${EXPOSURE}
        -Ofast
        ${NU_CLANG_COMMON_FLAGS}
      )
    else()
      # Debug.
      target_compile_definitions(${PROJECT_NAME} ${EXPOSURE}
        "_DEBUG=1"
        "DEBUG=1"
      )

      target_compile_options(${REQUIRED_ARG} ${EXPOSURE}
        -O0
        -g
        ${NU_CLANG_COMMON_FLAGS}
      )
    endif()

  #-------------------------------------------------------------
  # g++.
  #-------------------------------------------------------------
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if (CMAKE_BUILD_TYPE EQUAL "Release")
      # Release.
      target_compile_definitions(${PROJECT_NAME} ${EXPOSURE}
        "NDEBUG=1"
      )

      target_compile_options(${REQUIRED_ARG} ${EXPOSURE}
        -Ofast
        -stdlib=libc++
      )
    else()
      # Debug.
      target_compile_definitions(${PROJECT_NAME} ${EXPOSURE}
        "_DEBUG=1"
        "DEBUG=1"
      )

      target_compile_options(${REQUIRED_ARG} ${EXPOSURE}
        -stdlib=libc++
        -O0
        -g
      )
    endif()

  #-------------------------------------------------------------
  # Intel.
  #-------------------------------------------------------------
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    if (CMAKE_BUILD_TYPE EQUAL "Release")
      # Release.
      target_compile_definitions(${PROJECT_NAME} ${EXPOSURE}
        "NDEBUG=1"
      )

      target_compile_options(${REQUIRED_ARG} ${EXPOSURE}
        # Flags
      )
    else()
      # Debug.
      target_compile_definitions(${PROJECT_NAME} ${EXPOSURE}
        "_DEBUG=1"
        "DEBUG=1"
      )

      target_compile_options(${REQUIRED_ARG} ${EXPOSURE}
      # Flags
      )
    endif()

  #-------------------------------------------------------------
  # MSVC.
  #-------------------------------------------------------------
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(NU_MSVC_COMMON_FLAGS
      /Zc:alignedNew
    )

    set(NU_MSVC_COMMON_DEFS
      NOMINMAX
      UNICODE
      _UNICODE
      _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING
    )

    if (CMAKE_BUILD_TYPE EQUAL "Release")
      # Release.
      target_compile_definitions(${REQUIRED_ARG} ${EXPOSURE}
        ${NU_MSVC_COMMON_DEFS}
      )

      target_compile_options(${REQUIRED_ARG} ${EXPOSURE}
        ${NU_MSVC_COMMON_FLAGS}
      )
    else()
      # Debug.
      target_compile_definitions(${REQUIRED_ARG} ${EXPOSURE}
        ${NU_MSVC_COMMON_DEFS}
        "_DEBUG=1"
        "DEBUG=1"
      )

      target_compile_options(${REQUIRED_ARG} ${EXPOSURE}
        ${NU_MSVC_COMMON_FLAGS}
      )
    endif()
	endif()
endfunction()

# Conan search paths.
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR} ${CMAKE_PREFIX_PATH})

find_package(TBB CONFIG REQUIRED)

include(FetchContent)

set(FST_BUILD_TESTS OFF CACHE INTERNAL "")
set(FST_BUILD_BENCH OFF CACHE INTERNAL "")

# set(NU_BENCH_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/output/bench)

# set(FETCHCONTENT_SOURCE_DIR_FST ${CMAKE_CURRENT_SOURCE_DIR}/../fst CACHE INTERNAL "")

# if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../fst")
#   set(NU_FST_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../fst)
# endif()

FetchContent_Declare(
  fst
  GIT_REPOSITORY "https://github.com/alxarsenault/fst.git"
  # SOURCE_DIR ${NU_FST_SOURCE_DIR}
)

FetchContent_MakeAvailable(fst)


# Use folder.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

message("BUILD TYPE : " ${CMAKE_BUILD_TYPE})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-common -Wswitch -Wunused-variable -Wparentheses -Wnon-virtual-dtor -Wshorten-64-to-32 -Wundeclared-selector -Wuninitialized -Wunused-function -Wbool-conversion -Wcomma -Wconstant-conversion -Wempty-body -Wenum-conversion -Winfinite-recursion -Wint-conversion -Wrange-loop-analysis -Wstrict-prototypes -Wmove -Wunreachable-code -Wduplicate-method-match -Wreorder -fvisibility-inlines-hidden")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS}")

set(NU_BUILD_DIRECTORY ${CMAKE_SOURCE_DIR}/build/${CMAKE_SYSTEM_PROCESSOR}/${CMAKE_BUILD_TYPE})

# Debug output path.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/build/${CMAKE_SYSTEM_PROCESSOR}/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/build/${CMAKE_SYSTEM_PROCESSOR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/build/${CMAKE_SYSTEM_PROCESSOR}/Debug)

# Release output path.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/build/${CMAKE_SYSTEM_PROCESSOR}/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/build/${CMAKE_SYSTEM_PROCESSOR}/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/build/${CMAKE_SYSTEM_PROCESSOR}/Release)

set(NU_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(NU_SRC_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/)
set(NU_RESOURCES_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources)

# All sources.
file(GLOB_RECURSE NU_INCLUDE_FILES "${NU_INCLUDE_DIRECTORY}/*.h")
file(GLOB_RECURSE NU_SOURCE_FILES "${NU_SRC_DIRECTORY}/*.cpp" "${NU_SRC_DIRECTORY}/*.m" "${NU_SRC_DIRECTORY}/*.mm" "${NU_SRC_DIRECTORY}/*.h")

# File groups.
make_pretty_source_folders("include" "include" "${NU_INCLUDE_FILES}")
make_pretty_source_folders("src" "src" "${NU_SOURCE_FILES}")

# NU static library.
# add_library(${PROJECT_NAME} STATIC ${NU_INCLUDE_FILES} ${NU_SOURCE_FILES})

add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${NU_INCLUDE_FILES} ${NU_SOURCE_FILES})

nu_set_compile_options(${PROJECT_NAME} PRIVATE)

target_link_libraries(${PROJECT_NAME} PRIVATE
  fst
  "-framework CoreFoundation"
  "-framework Cocoa"
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
  NU_RESOURCES_DIRECTORY="${NU_RESOURCES_DIRECTORY}"
)
# target_link_libraries(${PLUGIN_NAME} PRIVATE
#     #AudioPluginData #If we 'd created a binary data target, we' d link to it here
#     juce::juce_audio_utils
#     dsp_kit
#     fst
#     juce_dsp_kit
#     TBB::TBB
#     "-framework Accelerate"
#     # ${PLUGIN_NAME}_embedded_data
# )

target_include_directories(${PROJECT_NAME} PUBLIC
  ${NU_SRC_DIRECTORY}
  ${NU_INCLUDE_DIRECTORY}
)

#-----------------------------------------
