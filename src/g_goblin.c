#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "simple_logger.h"
#include "g_control.h"
#include "g_entity.h"
#include "g_goblin.h"
#include "g_floor.h"
Entity *other;
glob_model_pool *pool;
Vector3D bboxmin, bboxmax;
float framechange;
void goblin_think(Entity *self){
	
	float attack_range = 3.0;
	Entity *target = get_nearest_target(self,other);
	if (target){
		if (self->overworld == 1){
			if (vector3d_magnitude_between(target->position, self->position) > 30){
				self->action = none;
			}
			else{
				self->action = movement;
			}
		}

		else{
			if (self->state != ES_Dying){
				if (vector3d_magnitude_between(target->position, self->position) > attack_range){
					//self->state = ES_Running;
					if (self->is_hit == 0){
						self->action = movement;
					}
				}
				else if (vector3d_magnitude_between(target->position, self->position) <= attack_range){
					//self->state = ES_Attacking;
					int rdm = (rand() % 11);
					if (rdm <= 7){
						if (self->overworld == 0){
							self->action = attack;
							//slog("attack");}
						}
					}
					else{
						self->action = none;
						//slog("idle");
					}
				}

			}
		}
	}
	//EntityState prev_state = self->state;
	if (self->in_action == false){
		switch (self->action){
		case attack:
			if (self->can_attack == true){
				goblin_attack(self);
			}
			break;
		case movement:
			if (self->state != ES_Running&&self->is_hit == false){
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
			goblin_displacement(self, vector3d(c.x*self->movementspeed, c.y*self->movementspeed,0));
			break;
		case block:
			if (self->can_block == true){
				goblin_block(self);
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
void goblin_attack(Entity *self){
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
void update_goblin_model(Entity *self){
	self->frame = 0;
	if (self->state == ES_Idle){
		self->model = self->mods.idle;
		self->can_attack = true;
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
		self->attacknum=0;
		}
	}
	if (self->state == ES_Dying){
		self->model = self->mods.dying;
		self->can_attack = false;
	}
}

void update_goblin_ent(Entity *self){
	if (self->overworld == 1){
		self->EntMatx[3][2] = return_terrain_height(&other[6], -self->position.x, self->position.y);
	}
	if (self->health <= 0){
		if (self->state != ES_Dying){
			self->state = ES_Dying;
			self->update_model(self);
		}
	}
	set_position(self, self->EntMatx);
	self->think(self);
	self->frame = self->frame + 0.9;
	if (self->state == ES_Attacking){
		if (round(self->frame) == 3){
			create_projectile_e(self, NULL, pool->hitbox, 5.0, 20, false, 1, vector3d(0, 0, 0));
		}
	}
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
		else if (self->state == ES_Dying){
			self->frame = self->model->frameCount-1;
			update_battle_manager(-1, 0);
			self->_inuse = 0;
			free_ent_manager(self->Ent_ID);
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

void goblin_block(Entity *self){
	if (self->can_block == true){
		self->state = ES_Blocking;
		self->attacknum = 0;
		//slog("state: blocking");
		self->update_model(self);
	}


}
void goblin_displacement(Entity *self, Vector3D disp){
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
		slog("position x:%f y:%f z:%f", self->position.x, self->position.y, self->position.z);
		//slog("up after x:%f y:%f z:%f frame:%f", self->up.x, self->up.y, self->up.z, framechange);
	}
}
void init_goblin_ent(Entity *self, int ctr, Entity *ents, glob_model_pool *pools){
	set_position(self, self->EntMatx);
	self->state = ES_Idle;
	self->dr = Up;
	self->attack=goblin_attack;
	self->block = goblin_block;
	self->update_model=update_goblin_model;
	self->update_ent=update_goblin_ent;
	self->think = goblin_think;
	self->can_attack = true;
	self->can_hpskill = true;
	self->can_special = true;
	self->can_block = true;
	self->is_hit = false;
	self->action = none;
	self->prev_action = none;
	self->up = vector3d(0, 1,0);
	//self->right = vector3d(0, 1, 0);
	self->controling = 0;
	self->movementspeed = .20;
	self->attackdmg = 5;
	self->rotated = 0.0f;
	self->overworld = ctr;
	self->type = ES_Enemy;
	self->health = 200;
	self->in_action = false;
	other = ents;
	pool = pools;
	self->mods.idle = gf3d_model_load_animated("//enemy//goblin//idle//goblin_idle", "//enemy//goblin", 0, 56);

	self->mods.run = gf3d_model_load_animated("//enemy//goblin//run//goblin_run", "//enemy//goblin", 0, 33);

	self->mods.hit = gf3d_model_load_animated("//enemy//goblin//hit//goblin_hit", "//enemy//goblin", 0, 21);

	self->mods.attack1 = gf3d_model_load_animated("//enemy//goblin//attack//goblin_attack", "//enemy//goblin", 0, 40);
	self->mods.dying = gf3d_model_load_animated("//enemy//goblin//dead//dying", "//enemy//goblin", 0, 53);
	self->update_model(self);
	//self->model->mesh[0]->minv.x = 3;
	gf3d_set_boundbox(self, self->model->mesh[0]->minv, self->model->mesh[0]->maxv);
}

/*eol@eof*/