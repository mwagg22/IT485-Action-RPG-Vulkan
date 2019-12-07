#include "gf3d_model.h"
#include "gf3d_commands.h"
#include "gf3d_vgraphics.h"
#include "gf3d_obj_load.h"
#include "g_entity.h"

extern Uint32 bufferFrame;
extern VkCommandBuffer commandBuffer;
void store_ents(Entity *entsToAdd);
void update_screen(Uint32 bufferFrame, VkCommandBuffer commandBuffer);