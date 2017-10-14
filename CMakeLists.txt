cmake_minimum_required(VERSION 3.8)
project(TrabCG)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(FREEGLUT REQUIRED)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)

if(NOT ${OPENGL_FOUND})
    message("OPENGL not found")
endif()

include_directories(
        ${PROJECT_SOURCE_DIR}
        ${FREEGLUT_INCLUDE_DIR}
        ${GLUT_INCLUDE_DIR}
        ${GLU_INCLUDE_DIR}
        ${OPENGL_INCLUDE_DIR}
)

add_executable(TrabCG ${SOURCE_FILES})
link_directories(${OPENGL_gl_LIBRARY})
target_link_libraries(${PROJECT_NAME}
        ${OPENGL_LIBRARY}
        ${FREEGLUT_LIBRARY}
        ${GLEW_LIBRARY}
        )