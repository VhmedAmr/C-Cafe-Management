cmake_minimum_required(VERSION 3.10)
project(CafeManagement)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compiler flags for MinGW
if(MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
endif()

# Set FLTK paths
set(FLTK_ROOT "C:/FLTK/fltk-1.4.2")
set(FLTK_INCLUDE_DIR "${FLTK_ROOT}")
set(FLTK_LIBRARY_DIR "${FLTK_ROOT}/build/lib")

# Set MariaDB paths
set(MARIADB_SERVER_ROOT "C:/Program Files/MariaDB 11.7")
set(MARIADB_CONNECTOR_ROOT "C:/Program Files (x86)/MariaDB/MariaDB Connector C")
set(MARIADB_INCLUDE_DIRS "${MARIADB_CONNECTOR_ROOT}/include")
set(MARIADB_LIBRARY_DIRS "${MARIADB_CONNECTOR_ROOT}/lib")

# Add executable
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/database.cpp
    src/custom_table.cpp
    src/views/auth_window.cpp
    src/views/menu_window.cpp
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${FLTK_ROOT}
    ${FLTK_ROOT}/FL
    ${FLTK_ROOT}/build
    ${FLTK_ROOT}/build/FL
    ${MARIADB_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
)

# Link directories
target_link_directories(${PROJECT_NAME} PRIVATE
    ${FLTK_LIBRARY_DIR}
    ${MARIADB_LIBRARY_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    fltk
    fltk_images
    fltk_forms
    "${MARIADB_LIBRARY_DIRS}/libmariadb.lib"
    ws2_32
    gdi32
    gdiplus
    ole32
    uuid
    comdlg32
    comctl32
    imm32
    winmm
    shlwapi
)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Copy required DLLs
if(WIN32)
    # Copy FLTK DLLs
    file(GLOB FLTK_DLLS "${FLTK_ROOT}/build/bin/*.dll")
    foreach(DLL ${FLTK_DLLS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${DLL}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    endforeach()
    
    # Copy MariaDB DLL
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${MARIADB_CONNECTOR_ROOT}/lib/libmariadb.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif() 