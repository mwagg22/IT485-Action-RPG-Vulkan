#ifndef NK_vulk_H_
#define NK_vulk_H_

#define WIN32_LEAN_AND_MEAN
#define COBJMACROS
#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include <stddef.h>
#include <string.h>
#include <float.h>
#include <assert.h>
#define NK_IMPLEMENTATION
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_INDEX_BUFFER 128 * 1024
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_VUL_IMPLEMENTATION
#define NK_NUKLEAR_H_

#include "nuklear.h"
//#include "nuklear_d3d11_vertex_shader.h"
//#include "nuklear_d3d11_pixel_shader.h"

struct nk_vulk_vertex {
	float position[2];
	float uv[2];
	nk_byte col[4];
};

typedef struct nk_vulk_S
{
	struct nk_buffer cmds;
	struct nk_draw_null_texture null;
	VkDevice logical_device;
	VkPhysicalDevice physical_device;
	VkQueue graphics_queue;
	uint32_t graphics_queue_index;
	VkFramebuffer* framebuffers;
	uint32_t framebuffers_len;
	VkFormat color_format;
	VkFormat depth_format;
	VkSemaphore render_completed;
	VkSampler font_tex;
	VkImage font_image;
	VkImageView font_image_view;
	VkDeviceMemory font_memory;
	VkPipelineLayout pipeline_layout;
	VkRenderPass render_pass;
	VkPipeline pipeline;
	VkBuffer vertex_buffer;
	VkDeviceMemory vertex_memory;
	VkBuffer index_buffer;
	VkDeviceMemory index_memory;
	VkBuffer uniform_buffer;
	VkDeviceMemory uniform_memory;
	VkCommandPool command_pool;
	VkCommandBuffer* command_buffers; // currently always length 1
	VkDescriptorPool descriptor_pool;
	VkDescriptorSetLayout descriptor_set_layout;
	VkDescriptorSet descriptor_set;

} nk_vulk;

static struct nk_sdl {
	SDL_Window *win;
	nk_vulk ogl;
	struct nk_context ctx;
	struct nk_font_atlas atlas;
} sdl;
enum nk_vulk_init_state{
	NK_VK_DEFAULT = 0,
	NK_VK_INSTALL_CALLBACKS
};
NK_API struct nk_context*   nk_vulk_init(SDL_Window *win, VkDevice logical_device, VkPhysicalDevice physical_device, VkQueue graphics_queue, uint32_t graphics_queue_index, VkFramebuffer* framebuffers, uint32_t framebuffers_len, VkFormat color_format, VkFormat depth_format, enum nk_vulk_init_state);
NK_API void                 nk_vulk_shutdown(void);
NK_API void                 nk_vulk_font_stash_begin(struct nk_font_atlas **atlas);
NK_API void                 nk_vulk_font_stash_end(void);
NK_API void                 nk_vulk_new_frame();
NK_API VkSemaphore          nk_vulk_render(enum nk_anti_aliasing AA, uint32_t buffer_index, VkSemaphore wait_semaphore);

NK_API void                 nk_vulk_device_destroy(void);
NK_API void                 nk_vulk_device_create(VkDevice, VkPhysicalDevice, VkQueue graphics_queue, uint32_t graphics_queue_index, VkFramebuffer* framebuffers, uint32_t framebuffers_len, VkFormat, VkFormat);

NK_API int                  nk_sdl_handle_event(SDL_Event *evt);
#endif