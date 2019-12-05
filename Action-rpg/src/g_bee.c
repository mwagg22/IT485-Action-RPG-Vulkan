#include <stdlib.h>
#include <string.h>

#include "simple_logger.h"
#include "g_control.h"
#include "g_entity.h"
#include "g_bee.h"

Entity *other;
glob_model_pool *pool;
Vector3D bboxmin, bboxmax;
float framechange;
void bee_think(Entity *self){
	
	float attack_range = 5.0;
	Entity *target = get_nearest_target(self,other);
	if (target){
		if (vector3d_magnitude_between(target->position, self->position) > attack_range){
			//self->state = ES_Running;
			if (self->is_hit == 0){
				self->action = movement;
			}
		}
		else if (vector3d_magnitude_between(target->position, self->position) <= attack_range){
			//self->state = ES_Attacking;
			int rdm=(rand() % 11);
			if (rdm <= 2){
				self->action = attack;
				//slog("attack");
			}
			else{
				self->action = none;
				//slog("idle");
			}
		}

	}
	//EntityState prev_state = self->state;
	if (self->in_action == false){
		switch (self->action){
		case attack:
			if (self->can_attack == true){
				bee_attack(self);
			}
			break;
		case movement:
			if (self->state != ES_Running&&self->is_hit==false){
				Vector3D d, e;
				vector3d_sub(d, self->position, target->position);
				d.x = -d.x;
				d.y = d.y;
				d.z = 0;
				e.x = 0;
				e.y = 0;
				e.z = 0;
				vector3d_normalize(&d);
				if (self->state = ES_Running){
					rotate_towards_target(self, d, &e);
				}
				self->state = ES_Running;
				self->update_model(self);
			}
			Vector3D c;
			vector3d_sub(c, self->position, target->position);
			c.x = -c.x;
			c.y = c.y;
			c.z = -c.z;
			vector3d_normalize(&c);
			bee_displacement(self, vector3d(c.x*self->movementspeed, c.y*self->movementspeed,0));
			break;
		case block:
			if (self->can_block == true){
				bee_block(self);
				self->in_action = true;
				//self->update_model(self);
			}
			break;
		case none:
			if (self->state != ES_Attacking){
				Vector3D d, e;
				vector3d_sub(d, self->position, target->position);
				d.x = -d.x;
				d.y = d.y;
				d.z = 0;
				e.x = 0;
				e.y = 0;
				e.z = 0;
				vector3d_normalize(&d);
				if (self->state = ES_Running){
					rotate_towards_target(self, d, &e);
				}
				self->state = ES_Idle;
				self->in_action = true;
				self->update_model(self);
			}
			break;
		default:
			break;
		}
	}
}
void bee_attack(Entity *self){
	switch (self->attacknum){
	case 0:
		if (self->can_attack == true){
			self->can_block = false;
			self->in_action = true;
				//slog("attack num1: %i", self->attacknum);
				self->can_attack = false;
				self->state = ES_Attacking;
				//update_model(self);
				self->update_model(self);
				//attacknum++;
		}
		break;
	default:
		slog("How i get here");
		break;

	}
}
void update_bee_model(Entity *self){
	self->frame = 0;
	if (self->state == ES_Idle){
		self->model = self->mods.idle;
		//self->can_attack = true;
		self->can_block = true;
	}
	if (self->state == ES_Running){
		self->model = self->mods.run;
	}
	if (self->state == ES_Blocking){
		self->model = self->mods.block;
		self->can_attack = false;
	}
	if (self->state == ES_Hit){
		self->model = self->mods.hit;
		self->can_attack = false;
	}
	if (self->state == ES_Attacking){
		if (self->attacknum == 0){
			//slog("attack1");
			self->can_attack = false;
			self->model = self->mods.attack1;
			create_projectile_e(self, NULL, pool->hitbox, 5.0, 20, false, 1, vector3d(0, 0, 0));
		self->attacknum=0;
		}
	}
}

void update_bee_ent(Entity *self){
	set_position(self, self->EntMatx);
	self->think(self);
	self->frame = self->frame + 0.9;
	if(self->frame>=26&&self->state==ES_Attacking){	
		self->can_block = true;
		//slog("can attack again attacknum:%i",attacknum);

}

	if (self->frame >= self->model->frameCount){
		if (self->state == ES_Attacking){
			self->state = ES_Idle;
			self->can_attack = true;
			self->can_block = true;
			self->attacknum = 0;
			self->update_model(self);
		}
		else if (self->state == ES_Hit){
			self->is_hit = false;
			self->state = ES_Idle;
			self->frame = 0;
			self->update_model(self);
		}
		else{
			self->can_attack = true;
			self->frame = 0;
			self->in_action = false;
		}
		//slog("frame hit 0. state num:%i",self->state);
	}
	//slog("Frame %f", self->frame);
	
}

void bee_block(Entity *self){
	if (self->can_block == true){
		self->state = ES_Blocking;
		self->attacknum = 0;
		//slog("state: blocking");
		self->update_model(self);
	}


}
void bee_displacement(Entity *self, Vector3D disp){
	//vector3d_normalize(&disp);
	vector3d_rotate_about_z(&disp, self->rotated);
	float angle;
	if (disp.x == 0 && disp.y == 0 && disp.z == 0){
		return;
	}
	else{
		angle = vector3d_angle_between_vectors(self->up, disp);
		//slog("angle:%f", angle);
		if (!self->is_hit){
			gfc_matrix_rotate(self->EntMatx, self->EntMatx, angle, vector3d(0, 0, 1));
			self->up.x = disp.x;
			self->up.y = disp.y;
			self->up.z = disp.z;
		}
		gfc_matrix_translate(self->EntMatx, disp);

		//slog("up after x:%f y:%f z:%f frame:%f", self->up.x, self->up.y, self->up.z, framechange);
	}
}
void init_bee_ent(Entity *self, int ctr, Entity *ents, glob_model_pool *pools){
	set_position(self, self->EntMatx);
	self->state = ES_Idle;
	self->dr = Up;
	self->attack=bee_attack;
	self->block = bee_block;
	self->update_model=update_bee_model;
	self->update_ent=update_bee_ent;
	self->think = bee_think;
	self->can_attack = true;
	self->can_hpskill = true;
	self->can_special = true;
	self->can_block = true;
	self->is_hit = false;
	self->action = none;
	self->prev_action = none;
	self->up = vector3d(0, 1,0);
	//self->right = vector3d(0, 1, 0);
	self->controling = ctr;
	self->movementspeed = .20;
	self->rotated = 0.0f;
	self->type = ES_Enemy;
	self->in_action = false;
	other = ents;
	pool = pools;
	self->mods.idle = gf3d_model_load_animated("//enemy//bee//idle//bee_idle", "//enemy//bee", 0, 42);

	self->mods.run = gf3d_model_load_animated("//enemy//bee//idle//bee_idle", "//enemy//bee", 0, 42);

	self->mods.hit = gf3d_model_load_animated("//enemy//bee//hit//bee_hit", "//enemy//bee", 0, 29);

	self->mods.attack1 = gf3d_model_load_animated("//enemy//bee//attack//bee_attack", "//enemy//bee", 0, 16);
	self->update_model(self);
	//self->model->mesh[0]->minv.x = 3;
	gf3d_set_boundbox(self, self->model->mesh[0]->minv, self->model->mesh[0]->maxv);
}

/*eol@eof*/