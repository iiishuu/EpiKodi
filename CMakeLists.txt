cmake_minimum_required(VERSION 3.16)
project(EpiKodi LANGUAGES CXX)

# --- Standard C++ ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Qt setup (Qt6) ---
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Try Qt6 first, fallback to Qt5
find_package(Qt6 COMPONENTS Core Widgets Multimedia MultimediaWidgets QUIET)
if (Qt6_FOUND)
    message(STATUS "Using Qt6")
    set(QT_VERSION_MAJOR 6)
else()
    find_package(Qt5 COMPONENTS Core Widgets Multimedia MultimediaWidgets REQUIRED)
    message(STATUS "Using Qt5")
    set(QT_VERSION_MAJOR 5)
endif()

# --- Output dir ---
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- Includes ---
include_directories(${CMAKE_SOURCE_DIR}/include)

# --- Sources (architecture modulaire) ---
file(GLOB_RECURSE SOURCES
    src/core/menu.cpp
    src/core/app_controller.cpp  
    src/media/*.cpp
    src/ui/*.cpp
    src/components/*.cpp
)

# --- Executable principal ---
add_executable(epikodi ${SOURCES} src/main.cpp)

# --- Executable console (tests) ---
file(GLOB_RECURSE CONSOLE_SOURCES 
    src/core/menu.cpp
    src/media/*.cpp
)
# add_executable(epikodi_console ${CONSOLE_SOURCES} tests/test_main.cpp)  # Désactivé pour l'instant

# --- Link Qt libs ---
if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(epikodi
        Qt6::Core
        Qt6::Widgets
        Qt6::Multimedia
        Qt6::MultimediaWidgets
    )
else()
    target_link_libraries(epikodi
        Qt5::Core
        Qt5::Widgets
        Qt5::Multimedia
        Qt5::MultimediaWidgets
    )
endif()

# --- Console version (désactivée) ---
# La version console sera réactivée plus tard si nécessaire

# --- Custom target: rebuild (clean + cmake + build) ---
add_custom_target(rebuild
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR} ${CMAKE_COMMAND} ..
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# --- Custom target: run ---
add_custom_target(run
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}/bin/epikodi
    DEPENDS epikodi
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# --- Combined target: build + run ---
add_custom_target(build_and_run
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target epikodi
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}/bin/epikodi
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
