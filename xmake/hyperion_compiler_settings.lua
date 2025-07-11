---@diagnostic disable: undefined-global,undefined-field,lowercase-global
local function _set_compile_definitions(target)
    if is_mode("release") then
        target:add("defines", "NDEBUG")
    else
        target:add("defines", "DEBUG")
    end

    if target:is_plat("windows") then
        target:add("defines", "_CRT_SECURE_NO_WARNINGS")
        target:add("defines", "NOMINMAX")
        target:add("defines", "WIN32_LEAN_AND_MEAN")
    end
end

local function _set_compile_options(target)
    if is_mode("release") then
        target:set("optimize", "aggressive")
        if target:has_tool("cxx", "cl") then
            -- do not enable avx512 on MSVC is it has an optimizer error
            -- that will cause a crash due to misalignment
            target:add(
                "vectorexts",
                "avx",
                "avx2",
                "mmx",
                "neon",
                "sse",
                "sse2",
                "sse3",
                "sse4.2")
        else
            target:add("vectorexts", "all")
        end
        target:set("strip", "all")
    else
        target:set("optimize", "none")
    end

    target:add("cxflags", "--verbose")

    if target:has_tool("cxx", "cl", "clang_cl") then
        target:add("cxflags", "/permissive-", { public = true })
        target:add("cxflags", "/Zc:preprocessor", { public = true })
        target:add("cxflags", "/Zc:rvalueCast", { public = true })
        target:add("cxflags", "/Zc:__cplusplus", { public = true })
        target:add("cxflags", "/wd5104", { public = true })
        target:add("cxflags", "/MP", { public = false })
        target:add("cxflags", "/sdl", { public = false })
    elseif target:has_tool("cxx", "clang", "clang++") then
        target:add("cxflags", "-fsized-deallocation", { public = true })
        target:add("cxflags", "-ferror-limit=0", { public = true })
    end
end

local function _enable_warnings(target)
    if target:has_tool("cxx", "cl", "clang_cl") then
        target:add("cxflags", "/WX", { public = false })
        target:add("cxflags", "/W4", { public = false })
    elseif target:has_tool("cxx", "clang", "clang++") then
        target:add("cxflags", "-Wall", { public = false })
        target:add("cxflags", "-Wextra", { public = false })
        target:add("cxflags", "-Wpedantic", { public = false })
        target:add("cxflags", "-Weverything", { public = false })
        target:add("cxflags", "-Werror", { public = false })
        target:add("cxflags", "-Wno-c++98-compat", { public = true })
        target:add("cxflags", "-Wno-c++98-compat-pedantic", { public = true })
        target:add("cxflags", "-Wno-c++98-c++11-c++14-compat-pedantic", { public = true })
        target:add("cxflags", "-Wno-c++20-compat", { public = true })
        target:add("cxflags", "-Wno-gnu-zero-variadic-macro-arguments", { public = true })
        target:add("cxflags", "-Wno-undefined-func-template", { public = true })
        target:add("cxflags", "-Wno-ctad-maybe-unsupported", { public = true })
        target:add("cxflags", "-Wno-global-constructors", { public = true })
        target:add("cxflags", "-Wno-exit-time-destructors", { public = true })
        target:add("cxflags", "-Wno-extra-semi", { public = true })
        target:add("cxflags", "-Wno-extra-semi-stmt", { public = true })
        target:add("cxflags", "-Wno-unused-local-typedef", { public = true })
        target:add("cxflags", "-Wno-undef", { public = true })
        target:add("cxflags", "-Wno-unknown-warning-option", { public = true })
        target:add("cxflags", "-Wno-deprecated-ofast", { public = true })
    else
        target:add("cxflags", "-Wall", { public = false })
        target:add("cxflags", "-Wextra", { public = false })
        target:add("cxflags", "-Wpedantic", { public = false })
        target:add("cxflags", "-Wconversion", { public = false })
        target:add("cxflags", "-Wshadow", { public = false })
        target:add("cxflags", "-Werror", { public = false })
        target:add("cxflags", "-Wno-c++20-compat", { public = false })
        target:add("cxflags", "-Wno-terminate", { public = false })
        target:add("cxflags", "-Wno-extra-semi", { public = false })
    end
end

function set_compiler_settings(target)
    _set_compile_options(target)
    _set_compile_definitions(target)
    _enable_warnings(target)
end
