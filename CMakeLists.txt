cmake_minimum_required(VERSION 3.24)

include(lib/cmake/OptimizeForArchitecture.cmake)

project(
    Rbp
    VERSION 0.1.0
    LANGUAGES C CXX
)

option(MINIMAL_BUILD "Builds only the library" OFF)

add_library(
    ${PROJECT_NAME}
    SHARED
        src/Rbp/Md5.c
        src/Rbp/Md5.h
        src/Rbp/Rbp.c
        src/Rbp/Rbp.h
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        LINKER_LANGUAGE C
        C_STANDARD 23 # required for _BitInt
        COMPILE_WARNING_AS_ERROR ON
)

OptimizeForArchitecture()

foreach(flag ${Vc_ARCHITECTURE_FLAGS})
    if(NOT ${flag} IN_LIST COMPILE_OPTIONS)
        target_compile_options(${PROJECT_NAME} PUBLIC ${flag})
    endif()
endforeach()

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

target_compile_options(${PROJECT_NAME} PUBLIC -Wno-macro-redefined -Wno-microsoft-static-assert)

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
        RBPINTERNAL
        RBPEXPORTS
)

target_compile_definitions(
    ${PROJECT_NAME}
    PUBLIC
        SIMDE_ENABLE_NATIVE_ALIASES
)

target_include_directories(${PROJECT_NAME} PUBLIC lib/simde)

include_directories(src)

if (NOT MINIMAL_BUILD)
    add_executable(
        ${PROJECT_NAME}Sample
            src/RbpSample/Main.c
    )

    add_dependencies(${PROJECT_NAME}Sample ${PROJECT_NAME})
    target_link_libraries(${PROJECT_NAME}Sample PRIVATE ${PROJECT_NAME})
endif()