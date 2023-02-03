include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

enable_testing()

Function(addCase name sourceFile)
    add_executable(${name} ${sourceFile})
    target_link_libraries(${name} GTest::gtest_main)
    set_property(TARGET ${name} PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/__test__)
    gtest_discover_tests(${name})
endfunction(addCase)

if(1)
    file(GLOB_RECURSE globalTestFiles CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/**/*test.cpp)

    foreach(testFile ${globalTestFiles})
        string(REGEX MATCH "src.*__test__.*" path ${testFile})
        string(REGEX REPLACE "(__test__/)|(.test.cpp)|(src.)|(core.)" "" path "${path}")
        string(REGEX REPLACE "/" "-" path "${path}")

        message("testFile: ${testFile}")
        message("path: ${path}")

        addCase(test__${path} ${testFile})
    endforeach()
endif()