//
// Created by 12445 on 2020/2/11.
//

#ifndef TUTORIAL05_TRIANGLE_DEVICE_H
#define TUTORIAL05_TRIANGLE_DEVICE_H

#include <vulkan_wrapper.h>
#include <vector>
#include <android/log.h>
#include "../utils/util.hpp"

class Device
{
public:
	bool initialized_;
	VkInstance instance_;
	VkPhysicalDevice gpuDevice_;
	VkDevice device_;
	uint32_t queueFamilyIndex_;
	VkSurfaceKHR surface_;
	VkQueue queue_;
	void createVulkanDevice( ANativeWindow* platformWindow, VkApplicationInfo* appInfo );
	void initInstance( std::vector<const char*> instance_extensions, ANativeWindow* platformWindow, VkApplicationInfo* appInfo );
	void initEnumerateDevice( );
};

#endif //TUTORIAL05_TRIANGLE_DEVICE_H
