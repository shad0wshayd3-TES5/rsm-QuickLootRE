-- set minimum xmake version
set_xmakever("2.8.2")

-- includes
includes("lib/commonlibsse")

-- set project
set_project("QuickLootRE")
set_version("2.17.0")
set_license("MIT")

-- set defaults
set_languages("c++23")
set_warnings("allextra")

-- add rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- set policies
set_policy("build.optimization.lto", true)
set_policy("package.requires_lock", true)

-- set configs
set_config("rex_toml", true)
set_config("skyrim_ae", true)

-- targets
target("QuickLootRE")
    -- add dependencies to target
    add_deps("commonlibsse")

    -- add commonlibsse plugin
    add_rules("commonlibsse.plugin", {
        name = "QuickLootRE",
        author = "ryan-rsm-mckenzie"
    })

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")

    -- add extra files
    add_extrafiles(".clang-format")
