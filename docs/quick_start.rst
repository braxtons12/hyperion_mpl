Quick Start Guide
*****************

hyperion::mpl supports both CMake and XMake, and incorporating it in your project is quick and
easy.

CMake
-----

hyperion::latform is easily incorporated into a raw CMake project with :cmake:`FetchContent` or
other methods like :cmake:`add_subdirectory`\. Example for :cmake:`FetchContent`\:

.. code-block:: cmake
    :caption: CMakeLists.txt
    :linenos:

    # Include FetchContent so we can use it
    include(FetchContent)

    # Declare the dependency on hyperion-utils and make it available for use
    FetchContent_Declare(hyperion_mpl
        GIT_REPOSITORY "https://github.com/braxtons12/hyperion_mpl"
        GIT_TAG "origin/main")
    FetchContent_MakeAvailable(hyperion_mpl)

    # For this example, we create an executable target and link hyperion::mpl to it
    add_executable(MyExecutable "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
    target_link_libraries(MyExecutable PRIVATE hyperion::mpl)

Note that hyperion::mpl depends on
`hyperion::platform <https://github.com/braxtons12/hyperion_platform>`_ for platform and feature
detection macros and other core utilities, `doctest <https://github.com/doctest/doctestdoctest>`_
for testing, and optionally depends on `Tracy <https://github.com/wolfpld/tracy>`_ for the profiling
macros `hyperion::platform` wraps in :cpp:`#include <hyperion/platform/def.h>`\ (set the option
:cmake:`HYPERION_ENABLE_TRACY` to :cmake:`ON` to enable these). By default, it will use
:cmake:`FetchContent` to obtain these dependencies, but you can disable this by setting
:cmake:`HYPERION_USE_FETCH_CONTENT` to :cmake:`OFF`\, in which case you will need to make sure each
package is findable via CMake's :cmake:`find_package`.

XMake
-----

XMake is a new(er) Lua-based build system with integrated package management. It is the preferred
way to use Hyperion packages. Example:

.. code-block:: lua
    :caption: xmake.lua
    :linenos:

    set_project("my_project")

    -- add the hyperion_packages git repository as an XMake repository
    add_repositories("hyperion https://github.com/braxtons12/hyperion_packages.git")

    -- add hyperion_mpl as a required dependency for the project
    add_requires("hyperion_mpl", {
        -- Don't verify the git commit. This is necessary because hyperion::mpl hasn't reached
        -- an official release yet, and thus doesn't have a stable git commit to track.
        -- This allows pulling directly from $HEAD
        verify = false,
    })
    
    -- For this example, we create an executable target and link hyperion::mpl to it
    target("my_executable")
        set_kind("binary")
        add_packages("hyperion_mpl")
