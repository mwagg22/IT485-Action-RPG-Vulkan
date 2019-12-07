#include <stdio.h>
#include<math.h>
#include "gf3d_obj_load.h"
#include "simple_logger.h"
#include "g_floor.h"
#include "g_entity.h"
#include "g_hud.h"
#include "g_text.h"
Entity *other;

typedef struct
{
	healthbar               *hud_list;
	Uint32                  max_huds;
}HudManager;

HudManager gf3d_huds = { 0 };

void gf3d_hud_manager_close()
{
	int i;
	for (i = 0; i < gf3d_huds.max_huds; i++)
	{
		gf3d_huds.hud_list[i]._inuse = 0;
	}
	memset(&gf3d_huds, 0, sizeof(healthbar));
	slog("model manager closed");
}

void gf3d_hud_manager_init(Uint32 max_hudBox)
{
	if (max_hudBox == 0)
	{
		slog("cannot intilizat model manager for 0 models");
		return;
	}

	gf3d_huds.hud_list = (healthbar *)gfc_allocate_array(sizeof(healthbar), max_hudBox);
	gf3d_huds.max_huds = max_hudBox;
	slog("huds manager initiliazed");
	//atexit(gf3d_text_manager_close);
}

healthbar * gf3d_hud_new()
{
	int i;
	for (i = 0; i < gf3d_huds.max_huds; i++)
	{
		if (!gf3d_huds.hud_list[i]._inuse)
		{
			slog("huds index:%i", i);
			memset(&gf3d_huds.hud_list[i], 0, sizeof(healthbar));
			//gf3d_model_delete(&gf3d_textbox.textBox_list[i]);
			gf3d_huds.hud_list[i]._inuse = 1;
			//gfc_matrix_identity(gf3d_huds.hud_list[i].EntMatrix);
			return &gf3d_huds.hud_list[i];
		}
	}
	slog("unable to make a new huds, out of space");
	return NULL;
}
void update_hud_model(Entity *self){
	
}
void update_hud_ent(Entity *self){

}
void init_hud_ent(Entity *self, int ctr, Entity *ents){
	self->state = ES_Idle;
	self->dr = Up;
	self->update_ent = update_hud_ent;
	self->can_attack = true;
	self->can_hpskill = true;
	self->can_special = true;
	self->can_block = true;
	self->is_hit = false;
	self->action = none;
	self->prev_action = none;
	self->up = vector3d(0, 1, 0);
	self->controling = 0;
	self->movementspeed = 1.0f;
	self->rotated = 0.0f;
	self->type = ES_Stage;
	self->model = gf3d_model_load_animated("//other//ui//face//sword//sword_face", "//other//ui//sword_face", 0, 2);
	gfc_matrix_rotate(self->EntMatx, self->EntMatx, -1.5708, vector3d(0, 0, 1));
}

healthbar* create_healthbar(char *model, char *texture,float x, float y, float z,Entity *target){
	healthbar* bar = gf3d_hud_new();
	slog("here");
	bar->model = gf3d_model_load_animated(model, texture, 0, 2);
	gfc_matrix_identity(bar->EntMatrix);
	gfc_matrix_rotate(bar->EntMatrix, bar->EntMatrix, -1.5708, vector3d(0, 0, 1));
	bar->position = vector3d(x, y, z);
	bar->target = target;
	return bar;
}
void hud_set_position(healthbar *self){
	self->EntMatrix[3][1] = self->target->EntMatx[3][1] + self->position.x;
	self->EntMatrix[3][0] = self->target->EntMatx[3][0] + self->position.y;
	self->EntMatrix[3][2] = self->target->EntMatx[3][2] + self->position.z;
}
void draw_huds(Uint32 bufferFrame, VkCommandBuffer commandBuffer){
	healthbar *b;
	for (int p = 0; p < gf3d_huds.max_huds; p++){
		if (gf3d_huds.hud_list[p]._inuse){
			b = &gf3d_huds.hud_list[p];			
			gf3d_ui_draw(b->model, bufferFrame, commandBuffer, b->EntMatrix, (Uint32)0);
			hud_set_position(b);
		}
	}
}