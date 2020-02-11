//
// Created by RIVER1016 on 2020/2/11.
//

#include "Device.hpp"

void Device::initInstance( std::vector< const char* > instance_extensions, ANativeWindow* platformWindow, VkApplicationInfo* appInfo )
{
	VkInstanceCreateInfo instanceCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = nullptr,
		.pApplicationInfo = appInfo,
		.enabledExtensionCount = static_cast< uint32_t >( instance_extensions.size( ) ),
		.ppEnabledExtensionNames = instance_extensions.data( ),
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = nullptr,
	};
	CALL_VK( vkCreateInstance( &instanceCreateInfo, nullptr, &instance_ ) );

	VkAndroidSurfaceCreateInfoKHR createInfo
	{
		.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
		.pNext = nullptr,
		.flags = 0,
		.window = platformWindow
	};

	CALL_VK( vkCreateAndroidSurfaceKHR( instance_, &createInfo, nullptr, &surface_ ) );
}

void Device::initEnumerateDevice( )
{
	uint32_t gpuCount = 0;
	CALL_VK( vkEnumeratePhysicalDevices( instance_, &gpuCount, nullptr ) );
	VkPhysicalDevice tmpGpus[gpuCount];
	CALL_VK( vkEnumeratePhysicalDevices( instance_, &gpuCount, tmpGpus ) );
	gpuDevice_ = tmpGpus[0];  // Pick up the first GPU Device

	// Find a GFX queue family
	uint32_t queueFamilyCount;
	vkGetPhysicalDeviceQueueFamilyProperties( gpuDevice_, &queueFamilyCount, nullptr );
	assert( queueFamilyCount );
	std::vector<VkQueueFamilyProperties> queueFamilyProperties( queueFamilyCount );
	vkGetPhysicalDeviceQueueFamilyProperties( gpuDevice_, &queueFamilyCount, queueFamilyProperties.data( ) );

	uint32_t queueFamilyIndex;
	for ( queueFamilyIndex = 0; queueFamilyIndex < queueFamilyCount; queueFamilyIndex ++ )
	{
		if ( queueFamilyProperties[queueFamilyIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT )
			break;
	}
	assert( queueFamilyIndex < queueFamilyCount );
	queueFamilyIndex_ = queueFamilyIndex;
}

void Device::createVulkanDevice( ANativeWindow* platformWindow, VkApplicationInfo* appInfo )
{
	std::vector<const char*> instance_extensions;
	std::vector<const char*> device_extensions;
	instance_extensions.push_back("VK_KHR_surface");
	instance_extensions.push_back("VK_KHR_android_surface");
	device_extensions.push_back("VK_KHR_swapchain");

	initInstance( instance_extensions, platformWindow, appInfo );
	initEnumerateDevice( );

	// Create a logical device (vulkan device)
	float priorities[] = {
			1.0f,
	};
	VkDeviceQueueCreateInfo queueCreateInfo{
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.queueCount = 1,
			.queueFamilyIndex = queueFamilyIndex_,
			.pQueuePriorities = priorities,
	};

	VkDeviceCreateInfo deviceCreateInfo{
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.pNext = nullptr,
			.queueCreateInfoCount = 1,
			.pQueueCreateInfos = &queueCreateInfo,
			.enabledLayerCount = 0,
			.ppEnabledLayerNames = nullptr,
			.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size()),
			.ppEnabledExtensionNames = device_extensions.data(),
			.pEnabledFeatures = nullptr,
	};

	CALL_VK(vkCreateDevice(gpuDevice_, &deviceCreateInfo, nullptr,
						   &device_));
	vkGetDeviceQueue(device_, queueFamilyIndex_, 0, &queue_);
}