#include <stdlib.h>
#include <string.h>

#include "simple_logger.h"

#include "g_entity.h"
float framechange;
//float frame;

void init_ent(Entity *self,int cont){
	self->state = ES_Idle;
	self->dr = Up;
	self->controling = cont;
	self->position = vector3d(0, 0, 0);
	self->movementspeed = 30.f;
	self->up = vector3d(0, 1, 0);
}
void update_projectile_model(Entity *self){
	self->frame = 0;
	if (self->state == ES_Idle){
		self->model = self->idle;
	}
}
void update_ent(Entity *self){
	set_position(self, self->EntMatx);
	self->frame = self->frame + 0.9;
	if (self->frame >= self->model->framecount)self->frame = 0;
}

/*eol@eof*/