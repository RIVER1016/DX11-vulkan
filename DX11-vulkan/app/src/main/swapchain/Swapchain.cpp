//
// Created by 12445 on 2020/2/11.
//

#include "Swapchain.hpp"

void VulkanSwapchainInfo::CreateSwapChain( ) {
    LOGI("->createSwapChain");

    // **********************************************************
    // Get the surface capabilities because:
    //   - It contains the minimal and max length of the chain, we will need it
    //   - It's necessary to query the supported surface format (R8G8B8A8 for
    //   instance ...)
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device.gpuDevice_, device.surface_,
                                                    &surfaceCapabilities);
    // Query the list of supported surface format and choose one we like
    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device.gpuDevice_, device.surface_,
                                            &formatCount, nullptr);
    VkSurfaceFormatKHR* formats = new VkSurfaceFormatKHR[formatCount];
    vkGetPhysicalDeviceSurfaceFormatsKHR(device.gpuDevice_, device.surface_,
                                            &formatCount, formats);
    LOGI("Got %d formats", formatCount);

    uint32_t chosenFormat;
    for (chosenFormat = 0; chosenFormat < formatCount; chosenFormat++) {
        if (formats[chosenFormat].format == VK_FORMAT_R8G8B8A8_UNORM) break;
    }
    assert(chosenFormat < formatCount);

    displaySize_ = surfaceCapabilities.currentExtent;
    displayFormat_ = formats[chosenFormat].format;

    VkSurfaceCapabilitiesKHR surfaceCap;
    CALL_VK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device.gpuDevice_,
                                                        device.surface_, &surfaceCap));
    assert(surfaceCap.supportedCompositeAlpha | VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR);

    // **********************************************************
    // Create a swap chain (here we choose the minimum available number of surface
    // in the chain)
    VkSwapchainCreateInfoKHR swapchainCreateInfo{
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = nullptr,
        .surface = device.surface_,
        .minImageCount = surfaceCapabilities.minImageCount,
        .imageFormat = formats[chosenFormat].format,
        .imageColorSpace = formats[chosenFormat].colorSpace,
        .imageExtent = surfaceCapabilities.currentExtent,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .imageArrayLayers = 1,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 1,
        .pQueueFamilyIndices = &device.queueFamilyIndex_,
        .compositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
        .presentMode = VK_PRESENT_MODE_FIFO_KHR,
        .oldSwapchain = VK_NULL_HANDLE,
        .clipped = VK_FALSE,
    };
    CALL_VK(vkCreateSwapchainKHR(device.device_, &swapchainCreateInfo, nullptr,
                                    &swapchain_));
    // Get the length of the created swap chain
    CALL_VK(vkGetSwapchainImagesKHR(device.device_, swapchain_,
                                        &swapchainLength_, nullptr));
    delete[] formats;
    LOGI("<-createSwapChain");
}

void VulkanSwapchainInfo::DeleteSwapChain( ) {
	for (int i = 0; i < swapchainLength_; i++) {
		vkDestroyFramebuffer(device.device_, framebuffers_[i], nullptr);
		vkDestroyImageView(device.device_, displayViews_[i], nullptr);
		vkDestroyImage(device.device_, displayImages_[i], nullptr);
	}
	vkDestroySwapchainKHR(device.device_, swapchain_, nullptr);
}

void VulkanSwapchainInfo::CreateFrameBuffers(VkRenderPass& renderPass,
                        VkImageView depthView = VK_NULL_HANDLE) {
	// query display attachment to swapchain
	uint32_t SwapchainImagesCount = 0;
	CALL_VK(vkGetSwapchainImagesKHR(device.device_, swapchain_,
									&SwapchainImagesCount, nullptr));
	displayImages_.resize(SwapchainImagesCount);
	CALL_VK(vkGetSwapchainImagesKHR(device.device_, swapchain_,
									&SwapchainImagesCount,
									displayImages_.data()));

	// create image view for each swapchain image
	displayViews_.resize(SwapchainImagesCount);
	for (uint32_t i = 0; i < SwapchainImagesCount; i++) {
		VkImageViewCreateInfo viewCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.pNext = nullptr,
			.image = displayImages_[i],
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = displayFormat_,
			.components =
				{
					.r = VK_COMPONENT_SWIZZLE_R,
					.g = VK_COMPONENT_SWIZZLE_G,
					.b = VK_COMPONENT_SWIZZLE_B,
					.a = VK_COMPONENT_SWIZZLE_A,
				},
			.subresourceRange =
				{
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1,
				},
			.flags = 0,
		};
		CALL_VK(vkCreateImageView(device.device_, &viewCreateInfo, nullptr,
								&displayViews_[i]));
	}
	// create a framebuffer from each swapchain image
	framebuffers_.resize(swapchainLength_);
	for (uint32_t i = 0; i < swapchainLength_; i++) {
		VkImageView attachments[2] = {
			displayViews_[i], depthView,
		};
		VkFramebufferCreateInfo fbCreateInfo{
			.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			.pNext = nullptr,
			.renderPass = renderPass,
			.layers = 1,
			.attachmentCount = 1,  // 2 if using depth
			.pAttachments = attachments,
			.width = static_cast<uint32_t>(displaySize_.width),
			.height = static_cast<uint32_t>(displaySize_.height),
		};
		fbCreateInfo.attachmentCount = (depthView == VK_NULL_HANDLE ? 1 : 2);

		CALL_VK(vkCreateFramebuffer(device.device_, &fbCreateInfo, nullptr,
									&framebuffers_[i]));
	}
}
