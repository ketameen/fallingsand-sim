cmake_minimum_required(VERSION 3.10)

project(fallingsand)


set(INCLUDE_DIR include)

set(IMGUI_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/imgui ${CMAKE_SOURCE_DIR}/include/imgui/backends )

set(SFML_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/SFML/include )

set(IMGUI_SFML_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/imgui-sfml )

set(SFML_LIB_DIR ${CMAKE_SOURCE_DIR}/include/SFML/lib )

# OpenGL
find_package(OpenGL REQUIRED)

set(OPENGL_LIB opengl32 )

file(GLOB SFML_LIB_FILES "*.a" PATH_SUFFIXES SFML_LIB_DIR)

file(GLOB SOURCE_FILE include/src/*.cpp )

file(GLOB IMGUI_SOURCE_FILES include/imgui/*.cpp include/imgui/backends/*opengl3*.cpp include/imgui/backends/*win32*.cpp include/imgui-sfml/*.cpp  )



set(SFML_LIBRARIES sfml-audio sfml-graphics sfml-network sfml-system sfml-window)

set(DWM_API_LIBRARY dwmapi)

add_executable(${PROJECT_NAME} main.cpp)


target_link_libraries(${PROJECT_NAME} PRIVATE ${SFML_LIBRARIES} ${DWM_API_LIBRARY} ${OPENGL_LIB} )

target_link_directories(${PROJECT_NAME} PRIVATE ${SFML_LIB_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR} ${SFML_INCLUDE_DIR} ${IMGUI_INCLUDE_DIR} ${IMGUI_SFML_INCLUDE_DIR})

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILE} ${IMGUI_SOURCE_FILES})

include_directories(${INCLUDE_DIR})
