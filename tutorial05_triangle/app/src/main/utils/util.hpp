#include <vulkan/vulkan.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

void process_command_line_args(struct sample_info &info, int argc,
                               char *argv[]);
bool memory_type_from_properties(struct sample_info &info, uint32_t typeBits,
                                 VkFlags requirements_mask,
                                 uint32_t *typeIndex);
VkResult init_global_layer_properties(struct sample_info &info);
void init_instance_extension_names(struct sample_info &info);
void init_device_extension_names(struct sample_info &info);
VkResult init_instance(struct sample_info &info, char const *const app_short_name);
VkResult init_enumerate_device(struct sample_info &info, uint32_t gpu_count);
void init_window_size(struct sample_info &info, int32_t default_width, int32_t default_height);
void init_connection(struct sample_info &info);
void init_window(struct sample_info &info);
void init_swapchain_extension(struct sample_info &info);
VkResult init_device(struct sample_info &info);
void init_command_pool(struct sample_info &info);
void init_command_buffer(struct sample_info &info);
void execute_begin_command_buffer(struct sample_info &info);
void init_device_queue(struct sample_info &info);
void init_depth_buffer(struct sample_info &info);
void init_uniform_buffer(struct sample_info &info);
void init_descriptor_and_pipeline_layouts(struct sample_info &info, bool use_texture,
                                          VkDescriptorSetLayoutCreateFlags descSetLayoutCreateFlags);
void init_renderpass(struct sample_info &info, bool include_depth, bool clear, VkImageLayout finalLayout);
void init_shaders(struct sample_info &info, const char *vertShaderText, const char *fragShaderText);
void init_framebuffers(struct sample_info &info, bool include_depth);
void init_vertex_buffer(struct sample_info &info, const void *vertexData, uint32_t dataSize, uint32_t dataStride,
                        bool use_texture);
void init_descriptor_pool(struct sample_info &info, bool use_texture);
void init_descriptor_set(struct sample_info &info, bool use_texture);
void init_pipeline_cache(struct sample_info &info);
void init_pipeline(struct sample_info &info, VkBool32 include_depth, VkBool32 include_vi);