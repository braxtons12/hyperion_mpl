cmake_minimum_required(VERSION 3.20)
project(hyperion_mpl LANGUAGES CXX)

option(HYPERION_ENABLE_TRACY "Enables Profiling with Tracy" OFF)
option(HYPERION_USE_FETCH_CONTENT "Enables FetchContent usage for getting dependencies" ON)

if(HYPERION_USE_FETCH_CONTENT)
    include(FetchContent)
endif()

if(HYPERION_USE_FETCH_CONTENT)
    FetchContent_Declare(doctest
        GIT_REPOSITORY "https://github.com/doctest/doctest"
        GIT_TAG "v2.4.11"
        )
    FetchContent_MakeAvailable(doctest)

    FetchContent_Declare(hyperion_platform
        GIT_REPOSITORY "https://github.com/braxtons12/hyperion_platform"
        GIT_TAG "origin/main"
        )
    FetchContent_MakeAvailable(hyperion_platform)
else()
    find_package(doctest REQUIRED)
endif()

if(${HYPERION_ENABLE_TRACY})
    if(HYPERION_USE_FETCH_CONTENT)
        FetchContent_Declare(Tracy
            GIT_REPOSITORY "https://github.com/wolfpld/tracy"
            GIT_TAG "v0.8.2"
            )

        FetchContent_MakeAvailable(Tracy)
    else()
        find_package(Tracy REQUIRED)
    endif()
    SET(TRACY_LINK_TARGET Tracy::TracyClient)
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/hyperion_compiler_settings.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/hyperion_enable_warnings.cmake)

set(HYPERION_PLATFORM_INCLUDE_DIRECTORIES
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
set(HYPERION_PLATFORM_INCLUDE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/include/hyperion"
)
set(HYPERION_PLATFORM_HEADERS
    "${HYPERION_PLATFORM_INCLUDE_PATH}/mpl/index.h"
    "${HYPERION_PLATFORM_INCLUDE_PATH}/mpl/list.h"
    "${HYPERION_PLATFORM_INCLUDE_PATH}/mpl/value.h"
    "${HYPERION_PLATFORM_INCLUDE_PATH}/mpl/algorithms.h"
    "${HYPERION_PLATFORM_INCLUDE_PATH}/mpl/algorithms/all_of.h"
    "${HYPERION_PLATFORM_INCLUDE_PATH}/mpl/concepts.h"
    "${HYPERION_PLATFORM_INCLUDE_PATH}/mpl/concepts/comparable.h"
    "${HYPERION_PLATFORM_INCLUDE_PATH}/mpl/concepts/operator_able.h"
    "${HYPERION_PLATFORM_INCLUDE_PATH}/mpl/concepts/std_supplemental.h"
    "${HYPERION_PLATFORM_INCLUDE_PATH}/mpl/type_traits.h"
    "${HYPERION_PLATFORM_INCLUDE_PATH}/mpl/type_traits/is_comparable.h"
    "${HYPERION_PLATFORM_INCLUDE_PATH}/mpl/type_traits/is_operator_able.h"
    "${HYPERION_PLATFORM_INCLUDE_PATH}/mpl/type_traits/std_supplemental.h"
)

add_library(hyperion_mpl INTERFACE)
add_library(hyperion::mpl ALIAS hyperion_mpl)
target_include_directories(
    hyperion_mpl
    INTERFACE
    ${HYPERION_PLATFORM_INCLUDE_DIRECTORIES}
)
target_sources(
    hyperion_mpl
    INTERFACE
    ${HYPERION_PLATFORM_HEADERS}
)
target_link_libraries(
    hyperion_mpl
    INTERFACE
    doctest::doctest
    hyperion::platform
    ${TRACY_LINK_TARGET}
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

set(HYPERION_PLATFORM_DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/docs/_build/html")
set(HYPERION_PLATFORM_DOXYGEN_HTML "${HYPERION_PLATFORM_DOXYGEN_OUTPUT_DIR}/index.html")

set(HYPERION_PLATFORM_DOCS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/docs")
set(HYPERION_PLATFORM_DOCS_FILES
    "${HYPERION_PLATFORM_DOCS_DIR}/index.rst"
    "${HYPERION_PLATFORM_DOCS_DIR}/quick_start.rst"
)

add_custom_command(
    OUTPUT ${HYPERION_PLATFORM_DOXYGEN_HTML}
    DEPENDS ${HYPERION_PLATFORM_HEADERS} ${DOCS_FILES}
    COMMAND sphix-build -M html . _build
    WORKING_DIRECTORY ${HYPERION_PLATFORM_DOCS_DIR}
    MAIN_DEPENDENCY
    COMMENT "Generating documentation with doxygen, sphinx, and breathe"
    VERBATIM
)

add_custom_target(hyperion_mpl_doxygen DEPENDS ${DOXYGEN_HTML})

include(GNUInstallDirs)
install(DIRECTORY ${HYPERION_PLATFORM_DOXYGEN_OUTPUT_DIR} DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(DIRECTORY include/ DESTINATION include)
install(TARGETS hyperion_mpl DESTINATION lib)
