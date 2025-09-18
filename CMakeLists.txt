cmake_minimum_required(VERSION 3.16)

project(race_to_vatican C)

# Build options
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Enable warnings
if (MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Sources (explicit for clarity/maintainability)
set(SRC_FILES
  src/main.c
  src/config.c
  src/audio.c
  src/dsp.c
  src/ui.c
  src/scene_manager.c
  src/net.c                    # <-- add
  src/scenes/scene_menu.c
  src/scenes/scene_level.c
  src/scenes/scene_victory.c
  src/scenes/scene_defeat.c    # <-- add
  src/scenes/scene_help.c
)


add_executable(${PROJECT_NAME} ${SRC_FILES})
target_compile_definitions(${PROJECT_NAME} PRIVATE _POSIX_C_SOURCE=200809L)
target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# --- Dependencies ---

# Threads / pthread
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

# libm (not needed on MSVC)
if (NOT MSVC)
  find_library(M_LIB m)
  if (M_LIB)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${M_LIB})
  endif()
endif()

# ncurses
# Prefer the built-in FindCurses
# find_package(Curses REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE ${CURSES_LIBRARIES})
# target_include_directories(${PROJECT_NAME} PRIVATE ${CURSES_INCLUDE_DIR})

# ncursesw (wide character support)
find_library(NCURSESW_LIB ncursesw REQUIRED)

if (NOT NCURSESW_LIB)
    message(FATAL_ERROR "ncursesw library not found — please install it (e.g., libncursesw5-dev or ncurses-devel).")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${NCURSESW_LIB})

# PortAudio
# Try pkg-config first, then fall back to a manual search.
find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
  pkg_check_modules(PORTAUDIO QUIET portaudio-2.0)
  if (PORTAUDIO_FOUND)
    target_include_directories(${PROJECT_NAME} PRIVATE ${PORTAUDIO_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PORTAUDIO_LIBRARIES})
  endif()
endif()

if (NOT PORTAUDIO_FOUND)
  # Common names: portaudio, portaudio_static
  find_library(PORTAUDIO_LIB
    NAMES portaudio portaudio_static
    HINTS
      /usr/lib /usr/local/lib /opt/local/lib /opt/homebrew/lib
  )
  find_path(PORTAUDIO_INC
    NAMES portaudio.h
    HINTS
      /usr/include /usr/local/include /opt/local/include /opt/homebrew/include
  )
  if (PORTAUDIO_LIB AND PORTAUDIO_INC)
    target_include_directories(${PROJECT_NAME} PRIVATE ${PORTAUDIO_INC})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PORTAUDIO_LIB})
  else()
    message(FATAL_ERROR "PortAudio not found. Install it (e.g., Homebrew: 'brew install portaudio') or set PORTAUDIO_LIB/PORTAUDIO_INC.")
  endif()
endif()

# On Apple, PortAudio may require CoreAudio frameworks during link if static
if (APPLE)
  # Only add if the library is static or CMake fails to pull frameworks via the dylib itself
  target_link_libraries(${PROJECT_NAME} PRIVATE
    "-framework CoreAudio"
    "-framework AudioToolbox"
    "-framework AudioUnit"
    "-framework CoreServices"
    "-framework Carbon"
  )
endif()

# Linker settings
if (UNIX AND NOT APPLE)
  # Nothing special for Linux beyond pthread/m/ncurses/portaudio
endif()

# Output name
set_target_properties(${PROJECT_NAME} PROPERTIES
  OUTPUT_NAME "race_to_vatican"
)

# Install rule (optional)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
