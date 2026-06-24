cmake_minimum_required(VERSION 3.16)
project(VaganteImitation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

file(GLOB SOURCES
    *.cpp
    *.h
)

if(MSVC)
    list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
else()
    list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/winMain.cpp")
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL2::SDL2
    SDL2_image::SDL2_image
    m
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
