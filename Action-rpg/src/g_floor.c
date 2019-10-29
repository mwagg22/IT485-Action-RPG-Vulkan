#include <stdlib.h>
#include <string.h>

#include "simple_logger.h"
#include "g_floor.h"
#include "g_entity.h"
Entity *other;
void init_floor_ent(Entity *self, int ctr, Entity *ents){
	self->state = ES_Idle;
	self->dr = Up;
	//self->attack = mage_attack;
	//self->block = mage_block;
	//self->update_model = update_mage_model;
	self->update_ent = update_ent;
	//self->think = mage_think;
	self->can_attack = true;
	self->can_hpskill = true;
	self->can_special = true;
	self->can_block = true;
	self->is_hit = false;
	self->action = none;
	self->prev_action = none;
	//self->get_inputs = mage_get_inputs;
	self->up = vector3d(0, 1, 0);
	self->controling = ctr;
	self->movementspeed = 1.0f;
	self->rotated = 0.0f;
	self->type = ES_Stage;
	self->model = gf3d_model_load_animated("//other//level//grass//grass_plane", "grass", 0, 2);
	other = ents;


}