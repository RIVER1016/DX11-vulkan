//
// Created by 12445 on 2020/2/11.
//

#ifndef DX11_VULKAN_SWAPCHAIN_HPP
#define DX11_VULKAN_SWAPCHAIN_HPP

#include <vulkan_wrapper.h>
#include <vector>
class Swapchain {
public:
    VkSwapchainKHR swapchain_;
    uint32_t swapchainLength_;

    VkExtent2D displaySize_;
    VkFormat displayFormat_;

    // array of frame buffers and views
    std::vector<VkImage> displayImages_;
    std::vector<VkImageView> displayViews_;
    std::vector<VkFramebuffer> framebuffers_;
    void CreateSwapChain( ){ };
    void DeleteSwapChain( ){ };
    void CreateFrameBuffers(VkRenderPass& renderPass,
                                VkImageView depthView = VK_NULL_HANDLE){ };
};


#endif //DX11_VULKAN_SWAPCHAIN_HPP
