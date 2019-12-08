#include "g_control.h"

Uint32 bufferFrame = 0;
VkCommandBuffer commandBuffer;
Entity *ents3[10];

void store_ents(Entity *entsToAdd){
	int done = 0;
	for (int i = 0; i < 10; i++){
		if (ents3[i] == NULL){
			ents3[i] = entsToAdd;
			return;

		}
	}
}
void update_screen(Uint32 bufferFrame, VkCommandBuffer commandBuffer){
			size_t n = sizeof(ents3) / sizeof(int);
			for (int p = 0; p < n; p++){
				if (ents3[p] == NULL)continue;
				gf3d_model_draw(ents3[p]->model, bufferFrame, commandBuffer, ents3[p]->EntMatx, (Uint32)ents3[p]->frame);
			}	
}