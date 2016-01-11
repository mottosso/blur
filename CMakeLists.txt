cmake_minimum_required (VERSION 2.8)
project (BLUR)

# Version
set (BLUR_VERSION_MAJOR 0)
set (BLUR_VERSION_MINOR 1)
set (BLUR_VERSION_PATCH 0)

include_directories(
    "src/c"
    "ext"
)

file(GLOB SOURCES "src/c/*.c")

# Enable C99 mode on C / Clang
if (CMAKE_C_COMPILER_ID MATCHES "Clang" OR CMAKE_C_COMPILER_ID MATCHES "GNU")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -Werror")

  # Solving the error: "undefined reference to `sqrt'"
  find_library(M_LIB m)

elseif (CMAKE_C_COMPILER_ID MATCHES "MSVC")
  # Expose Windows-only headers.
  include_directories("ext/windows")

  # Add Windows-port of getopt.
  # Source: https://gist.github.com/mottosso/01633387b3e7b7856e2a
  set(SOURCES ${SOURCES} "ext/windows/getopt.c")

  # Accept dangers of strncpy and others.
  ADD_DEFINITIONS(/D _CRT_SECURE_NO_WARNINGS)

else ()
  MESSAGE (WARNING "Unsupported compiler: ${CMAKE_C_COMPILER_ID}")
  MESSAGE (WARNING "If you are getting errors, try compiling with clang-3.1 or gnu-2.8.")
endif ()

add_executable(blur ${SOURCES})

target_link_libraries(blur ${M_LIB})