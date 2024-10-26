cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME VulkanLearning)
set(CMAKE_CXX_STANDARD 17)

project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} src/main.cpp src/lve_window.cpp src/app.cpp src/lve_device.cpp src/lve_pipeline.cpp src/lve_swapchain.cpp)

find_package(Vulkan REQUIRED)
add_definitions(-DGLFW_INCLUDE_VULKAN)
include_directories(${VULKAN_INCLUDE_DIRS})
link_directories(${VULKAN_LIBRARY_DIRS})

include(Dependency.cmake)

target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
    Vulkan::Vulkan
    ${DEP_LIBS}
)

add_dependencies(${PROJECT_NAME} ${DEP_LIST})