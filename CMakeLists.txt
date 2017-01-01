cmake_minimum_required(VERSION 3.2.2)

project(GuncraftExtractor)

# todo: make some Android-specific conditional statements
option(USE_TO_STRING_HACK "Use the std::to_string hackish implementation on Android NDK" OFF)

if(MSVC)
      set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O2 -EHsc")
else()
      set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wextra -Werror -pedantic -g -std=c++1y")
endif()

set(SOURCES
      "src/GunCraftExtractor.cpp"
      "src/WAV.cpp"
      "src/XNB.cpp"
      "src/lodepng/lodepng.cpp"
    )

set(HEADERS
      "include/GunCraftExtractor.h"
      "include/WAV.h"
      "include/XNB.h"
      "include/lodepng/lodepng.h"
    )

if(USE_TO_STRING_HACK)
	set(HEADERS
		${HEADERS}
		"include/to_string/to_string.h"
	   )
	add_definitions("-DUSE_TO_STRING_HACK")
endif()

add_executable(GunCraftExtractor ${SOURCES} ${HEADERS})
