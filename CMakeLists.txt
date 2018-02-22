cmake_minimum_required(VERSION 3.2.2)

project(GuncraftExtractor)

# todo: make some Android-specific conditional statements
option(USE_TO_STRING_HACK "Use the std::to_string hackish implementation on Android NDK" OFF)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")
endif()

include_directories(include)

add_executable(GunCraftExtractor
    src/GunCraftExtractor.cpp
    src/WAV.cpp
    src/XNB.cpp
    src/lodepng/lodepng.cpp

    include/GunCraftExtractor.h
    include/WAV.h
    include/XNB.h
    include/lodepng/lodepng.h
)

if (USE_TO_STRING_HACK)
    target_sources(GunCraftExtractor PRIVATE
        include/to_string/to_string.h
    )
    add_definitions(-DUSE_TO_STRING_HACK)
endif()
