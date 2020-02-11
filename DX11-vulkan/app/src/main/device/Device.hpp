//
// Created by 12445 on 2020/2/11.
//

#ifndef TUTORIAL05_TRIANGLE_DEVICE_H
#define TUTORIAL05_TRIANGLE_DEVICE_H

#include <vulkan_wrapper.h>

class Device {
public:
    bool initialized_;

    VkInstance instance_;
    VkPhysicalDevice gpuDevice_;
    VkDevice device_;
    uint32_t queueFamilyIndex_;

    VkSurfaceKHR surface_;
    VkQueue queue_;
};

#endif //TUTORIAL05_TRIANGLE_DEVICE_H
