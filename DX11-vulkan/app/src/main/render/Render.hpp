//
// Created by 12445 on 2020/2/11.
//

#ifndef DX11_VULKAN_RENDER_HPP
#define DX11_VULKAN_RENDER_HPP

#include <vulkan_wrapper.h>

class Render {
public:
    VkRenderPass renderPass_;
    VkCommandPool cmdPool_;
    VkCommandBuffer* cmdBuffer_;
    uint32_t cmdBufferLen_;
    VkSemaphore semaphore_;
    VkFence fence_;
};


#endif //DX11_VULKAN_RENDER_HPP
