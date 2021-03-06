project("HedgeTest")
    language("C")
    kind("ConsoleApp")
    
    targetdir("bin/%{cfg.platform}/%{cfg.buildcfg}")
    runpathdirs("bin/%{cfg.platform}/%{cfg.buildcfg}")
    includedirs("../HedgeLib/include")

    -- Create temporary test headers
    if not os.isfile("src/tmp_tests_depends.h") then
        io.writefile("src/tmp_tests_depends.h",
            "/* Put any #includes here that are required for the tests in tests/tmp_tests.h. */\n")
    end

    if not os.isfile("src/tests/tmp_tests.h") then
        io.writefile("src/tests/tmp_tests.h",
            "/* Put tests here that aren't to be included as part of the HedgeLib repository. */\n")
    end

    -- Link to HedgeLib
    links("HedgeLib")
    
    if LibType == "shared" then
        defines({ "HL_IS_DLL" })
    end
    
    -- Platform-Specifics
    if target == "windows" then
        defines("NOMINMAX")
    end
    
    files({ "src/**.c", "src/**.h" })
    
    -- MSC Optimization
    filter("toolset:msc")
        flags("MultiProcessorCompile")
    
    -- Debug Configuration
    filter("configurations:Debug*")
        defines("DEBUG")
        symbols("On")

    -- Release Configuration
    filter("configurations:Release*")
        defines("NDEBUG")
        optimize("Speed")
        flags("LinkTimeOptimization")
        
    -- x86
    filter("platforms:x86")
        architecture("x86")
        defines("x86")
        
    -- x64
    filter("platforms:x64")
        architecture("x86_64")
        defines("x64")
