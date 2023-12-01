cmake_minimum_required(VERSION 3.3)

project(tri)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
aux_source_directory("src" ALL_SOURCES)
aux_source_directory("src/include" ALL_INCLUDE)

# imgui
file(GLOB IMGUI "./source/imgui/*.cpp" "./source/imgui/*.h")
message(WARNING ${IMGUI})

add_executable(${PROJECT_NAME} ${ALL_SOURCES} ${ALL_INCLUDE} ${IMGUI})

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC src/include)
target_include_directories(${PROJECT_NAME} PUBLIC source/imgui)
target_link_libraries(${PROJECT_NAME} GL dl glfw)

add_custom_target(
  copy_shader_files
  COMMAND
    ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/src/shader
    ${CMAKE_CURRENT_BINARY_DIR}/shader
  COMMAND
    ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/src/assert
    ${CMAKE_CURRENT_BINARY_DIR}/assert)

add_dependencies(${PROJECT_NAME} copy_shader_files)
