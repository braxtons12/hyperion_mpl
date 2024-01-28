function(hyperion_enable_warnings TARGET)
    set(PUBLIC_VISIBILITY PUBLIC)
    set(ADD_PRIVATE_ITEMS TRUE)

    get_target_property(TARGET_TYPE ${TARGET} TYPE)
    if(${TARGET_TYPE} STREQUAL "INTERFACE_LIBRARY")
        set(PUBLIC_VISIBILITY INTERFACE)
        set(ADD_PRIVATE_ITEMS FALSE)
    endif()

    if(MSVC)
        if(${ADD_PRIVATE_ITEMS})
            target_compile_options(
                ${TARGET}
                PRIVATE
                /W4
                /WX
            )
        endif()

        target_compile_options(
            ${TARGET}
            ${PUBLIC_VISIBILITY}
            /wd5104
        )
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMILER_ID STREQUAL "clang")
        if(${ADD_PRIVATE_ITEMS})
            target_compile_options(
                ${TARGET}
                PRIVATE
                -Wall
                -Wextra
                -Wpedantic
                -Weverything
                -Werror
            )
        endif()

        target_compile_options(
            ${TARGET}
            ${PUBLIC_VISIBILITY}
            -Wno-c++98-compat
            -Wno-c++98-compat-pedantic
            -Wno-c++98-c++11-c++14-compat-pedantic
            -Wno-c++20-compat
            -Wno-gnu-zero-variadic-macro-arguments
            -Wno-undefined-func-template
            -Wno-ctad-maybe-unsupported
            -Wno-global-constructors
            -Wno-exit-time-destructors
            -Wno-extra-semi
            -Wno-extra-semi-stmt
            -Wno-unused-local-typedef
            -Wno-undef
            -Wno-unknown-warning-option
        )
    else()
        if(${ADD_PRIVATE_ITEMS})
            target_compile_options(
                ${TARGET}
                PRIVATE
                -Wall
                -Wextra
                -Wpedantic
                -Wshadow
                -Wconversion
                -Werror
            )
        endif()

        target_compile_options(
            ${TARGET}
            ${PUBLIC_VISIBILITY}
            -Wno-c++20-compat
            -Wno-terminate
            -Wno-extra-semi
        )
    endif()
endfunction()
