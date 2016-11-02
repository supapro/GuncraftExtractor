cmake_minimum_required(VERSION 3.2.2)

project(GuncraftExtractor)

if(MSVC)
      set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O2 -EHsc")
else()
      set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wextra -pedantic -std=c++1y")
endif()

set(SOURCES
      "src/main.cpp"
      "src/WAV.cpp"
      "src/XNB.cpp"
      "src/lodepng/lodepng.cpp"
        src/converters.cpp)

set(HEADERS
      "include/WAV.h"
      "include/XNB.h"
      "include/lodepng/lodepng.h"
        include/converters.h)

add_executable(GunCraftExtractor ${SOURCES} ${HEADERS})
