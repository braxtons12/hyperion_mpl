function(hyperion_enable_sanitizers TARGET)
    set(PUBLIC_VISIBILITY PUBLIC)

    get_target_property(TARGET_TYPE ${TARGET} TYPE)
    if(${TARGET_TYPE} STREQUAL "INTERFACE_LIBRARY")
        set(PUBLIC_VISIBILITY INTERFACE)
    endif()

    if(MSVC)
        target_compile_options(${TARGET} ${PUBLIC_VISIBILITY} /fsanitize=address)
        target_link_options(${TARGET} ${PUBLIC_VISIBILITY} /fsanitize=address)
    else()
        target_compile_options(${TARGET} ${PUBLIC_VISIBILITY} -fsanitize=address -fsanitize=undefined)
        target_link_options(${TARGET} ${PUBLIC_VISIBILITY} -fsanitize=address -fsanitize=undefined)
    endif
endfunction()
