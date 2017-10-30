cmake_minimum_required(VERSION 3.8)

set(PROJ_NAME St-CCompiler)
project(${PROJ_NAME})

aux_source_directory(src SOURCE_DIR)

add_executable(${PROJ_NAME} ${SOURCE_DIR})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJ_NAME})
