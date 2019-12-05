#include <stdio.h>
#include<math.h>
#include "gf3d_obj_load.h"
#include "simple_logger.h"
#include "g_floor.h"
#include "g_entity.h"
Entity *other;

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
void hud_set_position(Entity *self, Matrix4 mat,Entity *ent,int x,int y,int z){
	self->EntMatx[3][1] = mat[3][1] + x;
	self->EntMatx[3][0] = mat[3][0] + y;
	self->EntMatx[3][2] = mat[3][2]+z ;

}
void hud_displacement(Entity *self, Vector3D disp){

}