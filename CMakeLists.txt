cmake_minimum_required(VERSION 3.16)
project(awatari LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(awatari src/main.cpp)
add_subdirectory(src)

target_link_libraries(awatari PRIVATE sfml-graphics)
target_link_libraries(awatari PRIVATE sfml-audio)
target_compile_features(awatari PRIVATE cxx_std_17)

target_include_directories(awatari PRIVATE include)
target_include_directories(awatari PRIVATE include/Awatari)
target_include_directories(awatari PRIVATE include/Awatari/CPU)
target_include_directories(awatari PRIVATE include/Awatari/Memory)
target_include_directories(awatari PRIVATE include/Awatari/Video)

target_precompile_headers(awatari PRIVATE include/stdafx.h)

if(WIN32)
    add_custom_command(
        TARGET awatari
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:awatari>
        VERBATIM)
endif()

install(TARGETS awatari)