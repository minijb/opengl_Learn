cmake_minimum_required(VERSION 3.3)

project(tri)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
aux_source_directory("src" ALL_SOURCES)
aux_source_directory("src/include" ALL_INCLUDE)
add_executable(${PROJECT_NAME} ${ALL_SOURCES} ${ALL_INCLUDE})

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC src/include)
target_link_libraries(${PROJECT_NAME} GL dl glfw)

add_custom_target(
  copy_shader_files
  ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/src/shader
  ${CMAKE_CURRENT_BINARY_DIR}/shader)

add_dependencies(${PROJECT_NAME} copy_shader_files)
