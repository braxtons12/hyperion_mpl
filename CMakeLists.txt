cmake_minimum_required(VERSION 3.20)
project(hyperion_mpl LANGUAGES CXX VERSION 0.9.2)

include(CTest)

option(HYPERION_ENABLE_TRACY "Enables Profiling with Tracy" OFF)
option(HYPERION_USE_FETCH_CONTENT "Enables FetchContent usage for getting dependencies" ON)

set(HYPERION_ENABLE_TRACY
    ${HYPERION_ENABLE_TRACY}
    CACHE
    INTERNAL
    "force dependencies's `HYPERION_ENABLE_TRACY` to the value of this top-level project")
set(HYPERION_USE_FETCH_CONTENT
    ${HYPERION_USE_FETCH_CONTENT}
    CACHE
    INTERNAL
    "force depedencies's `HYPERION_USE_FETCH_CONTENT` to the value of this top-level project")

if(HYPERION_USE_FETCH_CONTENT)
    include(FetchContent)
endif()

if(${HYPERION_USE_FETCH_CONTENT})
    FetchContent_Declare(hyperion_platform
        GIT_REPOSITORY "https://github.com/braxtons12/hyperion_platform"
        GIT_TAG "v0.5.3"
        SYSTEM
    )
    FetchContent_MakeAvailable(hyperion_platform)
else()
    find_package(hyperion_platform REQUIRED)
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/hyperion_compiler_settings.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/hyperion_enable_warnings.cmake)

set(HYPERION_MPL_INCLUDE_DIRECTORIES
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
set(HYPERION_MPL_INCLUDE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/include/hyperion"
)
set(HYPERION_MPL_HEADERS
    "${HYPERION_MPL_INCLUDE_PATH}/mpl.h"
    "${HYPERION_MPL_INCLUDE_PATH}/mpl/list.h"
    "${HYPERION_MPL_INCLUDE_PATH}/mpl/metapredicates.h"
    "${HYPERION_MPL_INCLUDE_PATH}/mpl/metatypes.h"
    "${HYPERION_MPL_INCLUDE_PATH}/mpl/pair.h"
    "${HYPERION_MPL_INCLUDE_PATH}/mpl/type.h"
    "${HYPERION_MPL_INCLUDE_PATH}/mpl/value.h"
    "${HYPERION_MPL_INCLUDE_PATH}/mpl/concepts.h"
    "${HYPERION_MPL_INCLUDE_PATH}/mpl/concepts/comparable.h"
    "${HYPERION_MPL_INCLUDE_PATH}/mpl/concepts/operator_able.h"
    "${HYPERION_MPL_INCLUDE_PATH}/mpl/concepts/std_supplemental.h"
    "${HYPERION_MPL_INCLUDE_PATH}/mpl/type_traits.h"
    "${HYPERION_MPL_INCLUDE_PATH}/mpl/type_traits/is_comparable.h"
    "${HYPERION_MPL_INCLUDE_PATH}/mpl/type_traits/is_operator_able.h"
    "${HYPERION_MPL_INCLUDE_PATH}/mpl/type_traits/std_supplemental.h"
)

add_library(hyperion_mpl INTERFACE)
add_library(hyperion::mpl ALIAS hyperion_mpl)
target_include_directories(
    hyperion_mpl
    INTERFACE
    ${HYPERION_MPL_INCLUDE_DIRECTORIES}
)
target_sources(
    hyperion_mpl
    INTERFACE
    ${HYPERION_MPL_HEADERS}
)
target_link_libraries(
    hyperion_mpl
    INTERFACE
    hyperion::platform
)

hyperion_compile_settings(hyperion_mpl)
hyperion_enable_warnings(hyperion_mpl)

add_executable(hyperion_mpl_main ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(hyperion_mpl_main
    PRIVATE
    hyperion::mpl
)

hyperion_compile_settings(hyperion_mpl_main)
hyperion_enable_warnings(hyperion_mpl_main)

add_test(NAME hyperion_mpl_main
         COMMAND hyperion_mpl_main)

set(HYPERION_MPL_DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/docs/_build/html")
set(HYPERION_MPL_DOXYGEN_HTML "${HYPERION_MPL_DOXYGEN_OUTPUT_DIR}/index.html")

set(HYPERION_MPL_DOCS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/docs")
set(HYPERION_MPL_DOCS_FILES
    "${HYPERION_MPL_DOCS_DIR}/index.rst"
    "${HYPERION_MPL_DOCS_DIR}/quick_start.rst"
    "${HYPERION_MPL_DOCS_DIR}/concepts.rst"
    "${HYPERION_MPL_DOCS_DIR}/list.rst"
    "${HYPERION_MPL_DOCS_DIR}/metapredicates.rst"
    "${HYPERION_MPL_DOCS_DIR}/metatypes.rst"
    "${HYPERION_MPL_DOCS_DIR}/pair.rst"
    "${HYPERION_MPL_DOCS_DIR}/type.rst"
    "${HYPERION_MPL_DOCS_DIR}/type_traits.rst"
    "${HYPERION_MPL_DOCS_DIR}/value.rst"
    "${HYPERION_MPL_DOCS_DIR}/concepts/comparable.rst"
    "${HYPERION_MPL_DOCS_DIR}/concepts/operator_able.rst"
    "${HYPERION_MPL_DOCS_DIR}/concepts/std_supplemental.rst"
    "${HYPERION_MPL_DOCS_DIR}/type_traits/is_comparable.rst"
    "${HYPERION_MPL_DOCS_DIR}/type_traits/is_operator_able.rst"
    "${HYPERION_MPL_DOCS_DIR}/type_traits/std_supplemental.rst"
)

add_custom_command(
    OUTPUT ${HYPERION_MPL_DOXYGEN_HTML}
    DEPENDS ${HYPERION_MPL_HEADERS} ${DOCS_FILES}
    COMMAND sphix-build -M html . _build
    WORKING_DIRECTORY ${HYPERION_MPL_DOCS_DIR}
    MAIN_DEPENDENCY
    COMMENT "Generating documentation with doxygen, sphinx, and breathe"
    VERBATIM
)

add_custom_target(hyperion_mpl_doxygen DEPENDS ${DOXYGEN_HTML})

include(GNUInstallDirs)
install(DIRECTORY ${HYPERION_MPL_DOXYGEN_OUTPUT_DIR} DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(DIRECTORY include/ DESTINATION include)
install(TARGETS hyperion_mpl DESTINATION lib)
