//
// Created by 12445 on 2020/2/11.
//

#ifndef DX11_VULKAN_GFXPIPELINE_HPP
#define DX11_VULKAN_GFXPIPELINE_HPP

#include <vulkan_wrapper.h>

class GfxPipeline {
public:
	VkPipelineLayout layout_;
	VkPipelineCache cache_;
	VkPipeline pipeline_;
};


#endif //DX11_VULKAN_GFXPIPELINE_HPP
