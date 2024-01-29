---@diagnostic disable: undefined-global,undefined-field
set_project("hyperion_mpl")
set_version("0.1.0")

set_xmakever("2.8.2")

set_languages("cxx20")

add_rules("mode.debug", "mode.release")
add_moduledirs("xmake")
add_repositories("hyperion https://github.com/braxtons12/hyperion_packages.git")

add_requires("hyperion_platform", {
    system = false,
    external = true,
    configs = {
        languages = "cxx20"
    }
})
add_requires("doctest", {
    system = false,
    external = true,
    configs = {
        languages = "cxx20"
    }
})

option("hyperion_enable_tracy", function()
    add_defines("TRACY_ENABLE")
    set_default(false)
    after_check(function(opt)
        if opt:enabled() then
            add_requires("tracy", {
                system = false,
                external = true,
                configs = {
                    languages = "cxx20"
                }
            })
        end
    end)
end)

local hyperion_mpl_headers = {
    "$(projectdir)/include/hyperion/mpl/algorithms.h",
    "$(projectdir)/include/hyperion/mpl/concepts.h",
    "$(projectdir)/include/hyperion/mpl/index.h",
    "$(projectdir)/include/hyperion/mpl/list.h",
    "$(projectdir)/include/hyperion/mpl/type_traits.h",
    "$(projectdir)/include/hyperion/mpl/value.h",
}
local hyperion_mpl_algorithms_headers = {
    "$(projectdir)/include/hyperion/mpl/algorithms/all_of.h",
}
local hyperion_mpl_concepts_headers = {
    "$(projectdir)/include/hyperion/mpl/concepts/comparable.h",
    "$(projectdir)/include/hyperion/mpl/concepts/operator_able.h",
    "$(projectdir)/include/hyperion/mpl/concepts/std_supplemental.h",
}
local hyperion_mpl_type_traits_headers = {
    "$(projectdir)/include/hyperion/mpl/type_traits/is_comparable.h",
    "$(projectdir)/include/hyperion/mpl/type_traits/is_operator_able.h",
    "$(projectdir)/include/hyperion/mpl/type_traits/std_supplemental.h",
}

target("hyperion_mpl", function()
    set_kind("headeronly")
    set_languages("cxx20")
    add_includedirs("$(projectdir)/include", { public = true })
    add_headerfiles(hyperion_mpl_headers, { prefixdir = "hyperion/mpl", public = true })
    add_headerfiles(hyperion_mpl_algorithms_headers, { prefixdir = "hyperion/mpl/algorithms", public = true })
    add_headerfiles(hyperion_mpl_concepts_headers, { prefixdir = "hyperion/mpl/concepts", public = true })
    add_headerfiles(hyperion_mpl_type_traits_headers, { prefixdir = "hyperion/mpl/type_traits", public = true })
    set_default(true)
    on_config(function(target)
        import("hyperion_compiler_settings", { alias = "settings" })
        settings.set_compiler_settings(target)
    end)
    add_options("hyperion_enable_tracy")

    add_packages("doctest", { public = true })
    add_packages("hyperion_platform", { public = true })
    if has_package("tracy") then
        add_packages("tracy", { public = true })
    end
end)

target("hyperion_mpl_main", function()
    set_kind("binary")
    set_languages("cxx20")
    add_files("$(projectdir)/src/main.cpp", { prefixdir = "hyperion/mpl" })
    add_deps("hyperion_mpl")
    set_default(true)
    on_config(function(target)
        import("hyperion_compiler_settings", { alias = "settings" })
        settings.set_compiler_settings(target)
    end)
end)

target("hyperion_mpl_docs", function()
    set_kind("phony")
    set_default(false)
    on_build(function(_)
        local old_dir = os.curdir()
        os.cd("$(projectdir)/docs")
        os.exec("sphinx-build -M html . _build")
        os.cd(old_dir)
    end)
end)
