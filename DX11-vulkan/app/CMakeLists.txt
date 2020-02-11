cmake_minimum_required(VERSION 3.4.1)

# build native_app_glue as a static lib
set(APP_GLUE_DIR ${ANDROID_NDK}/sources/android/native_app_glue)
include_directories(${APP_GLUE_DIR})
add_library( app-glue STATIC ${APP_GLUE_DIR}/android_native_app_glue.c)

# build vulkan app
set(SRC_DIR src/main/jni)
set(DEVICE_DIR src/main/device)
set(SWAPCHAN_DIR src/main/swapchain)
set(BUFFERS_DIR src/main/buffers)
set(GFXPIPELINE_DIR src/main/gfxPipeline)
set(RENDER_DIR src/main/render)
set(WRAPPER_DIR ../../common/vulkan_wrapper)

add_library(vktuts SHARED
            ${SRC_DIR}/VulkanMain.cpp
            ${SRC_DIR}/AndroidMain.cpp
            ${DEVICE_DIR}/Device.cpp
            ${SWAPCHAN_DIR}/Swapchain.cpp
            ${BUFFERS_DIR}/Buffers.cpp
            ${GFXPIPELINE_DIR}/GfxPipeline.cpp
            ${RENDER_DIR}/Render.cpp
            ${WRAPPER_DIR}/vulkan_wrapper.cpp)

include_directories(${WRAPPER_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror \
                     -DVK_USE_PLATFORM_ANDROID_KHR")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")
target_link_libraries(vktuts app-glue log android)
