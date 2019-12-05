#include <stdlib.h>
#include <string.h>

#include "simple_logger.h"

#include "g_entity.h"
#include "g_weapon.h"

bool in_attack_frame=false;
float framechange;
SDL_Event event;
float dx = 0;
float dy = 0;
float dz = 0;

void weapon_think(Entity *self){
	EntityState prev_state = self->state;
	
	switch (self->action){
	case attack:
		if (self->can_attack == true){
			slog("attacknum b4:%i", attacknum);
			sword_attack(self);
			attacknum++;
			slog("attacknum af:%i", attacknum);
			prev_action = self->action;
		}
		break;
	case movement:
		if (self->action != prev_action){
			self->update_model(self);
			prev_action = self->action;
		}
		break;
	default:
		break;
	}
	//self->action = none;
}
void weapon_attack(Entity *self){
	switch (attacknum){
		slog("attack num: %i", attacknum);
	case 0:
		if (self->can_attack == true){
			slog("attack num1: %i", attacknum);
			self->can_attack = false;
			self->state = ES_Attacking;
			//update_model(self);
			self->update_model(self);			
			//attacknum++;
		}
		break;
	case 1:
		if (self->can_attack == true){
			slog("attack num2: %i", attacknum);
			self->state = ES_Attacking;
			//update_model(self);
			self->update_model(self);
			self->can_attack = false;
			//attacknum++;
		}
	case 2:
		if (self->can_attack == true){
			slog("attack num3: %i", attacknum);
			self->state = ES_Attacking;
			//update_model(self);
			self->update_model(self);
			self->can_attack = false;
			//attacknum = 0;
		}
		break;
	default:
		slog("How i get here");
		break;

	}
}
void update_weapon_model(Entity *self){
	self->frame = 0;
	if (self->state == ES_Idle){
		self->model = gf3d_model_load_animated("sword_idle", 0, 29);
	}
	if (self->state == ES_Running){
		self->model = gf3d_model_load_animated("sword_run", 0, 29);
	}
	if (self->state == ES_Attacking){
		if (attacknum == 0){
			slog("attack1");
		self->model = gf3d_model_load_animated("sword_attack1", 0, 29);
		//attacknum++;
		}
		else if(attacknum==1){
			slog("attack2");
		self->model = gf3d_model_load_animated("sword_attack2", 0, 29);
		//attacknum++;
		}
		else if(attacknum==2){
			slog("attack3");
		self->model = gf3d_model_load_animated("sword_attack3", 0, 29);
		//attacknum = 0;
		}
	}
}
void update_weapon_ent(Entity *self){
	if (attacknum > 2)attacknum = 0;
	set_position(self, self->EntMatx);
	self->frame = self->frame + 0.9;
	if(self->frame>=24&&self->state==ES_Attacking){
		self->can_attack = true;
		slog("can attack again attacknum:%i",attacknum);

}
	if (self->frame >= 27 && self->state == ES_Attacking){
	self->state=ES_Idle;
	attacknum = 0;
	slog("state: %i", ES_Attacking);
	self->update_model(self);
}
	if (self->frame >= 28){
		self->frame = 0;
		//slog("frame hit 0. state num:%i",self->state);
	}
	//slog("Frame %f", self->frame);
	
}

void init_weapon_ent(Entity *self,int ctr){
	self->state = ES_Idle;
	self->dr = Up;
	self->attack=weapon_attack;
	self->update_model=update_weapon_model;
	self->update_ent=update_weapon_ent;
	self->think = weapon_think;
	self->can_attack = true;
	self->action = none;
	self->prev_action = none;
	self->get_inputs = sword_get_inputs;
	self->up = vector3d(0, 1,0);
	self->movementspeed = 30.f;
	self->rotated = 0.0f;
}

void weapon_displacement(Entity *self, Vector3D disp){
	vector3d_normalize(&disp);
	vector3d_rotate_about_z(&disp, self->rotated);
	float angle;
	if (disp.x == 0 && disp.y == 0 && disp.z == 0){
		//slog("no movement");
		return;
	}
	else{
		//slog("rotaiton");
		//slog("movemnt x:%f y:%f z:%f", disp.x, disp.y, disp.z);
		angle = vector3d_angle_between_vectors(self->up, disp);
		//slog("angle %f", angle);
		slog("up before x:%f y:%f z:%f frame:%f", self->up.x, self->up.y, self->up.z, framechange);
		gfc_matrix_rotate(self->EntMatx, self->EntMatx, angle, vector3d(0, 0, 1));
		gfc_matrix_translate(self->EntMatx, disp);
		self->up.x = disp.x;
		self->up.y = disp.y;
		self->up.z = disp.z;
		slog("up after x:%f y:%f z:%f frame:%f", self->up.x, self->up.y, self->up.z, framechange);
	}
}

void weapon_get_inputs(Entity *self, const Uint8 *keys, float delta){
	framechange = delta;

	EntityState prev_state = self->state;
	self->prev_action = self->action;
	if (self->controling == 1){
		if (self->state != ES_Attacking){
			if (keys[SDL_SCANCODE_RIGHT]){
				self->state = ES_Running;
				//update_model(self);
				self->dr = Right;
				dx = -1;
				//sword_displacement(self, vector3d(-self->movementspeed * framechange, 0,0));
				self->action = movement;
			}
			if (keys[SDL_SCANCODE_LEFT]){
				self->state = ES_Running;
				//update_model(self);
				self->dr = Left;
				dx = 1;
				//sword_displacement(self, vector3d(self->movementspeed * framechange, 0, 0 ));
				//gfc_matrix_translate(Ent.EntMatx, vector3d(1, 0, 0));
				self->action = movement;
			}
			if (keys[SDL_SCANCODE_UP]){
				self->state = ES_Running;
				//update_model(self);
				self->dr = Up;
				dy = 1;
				//sword_displacement(self, vector3d(0, self->movementspeed * framechange, 0));
				//gfc_matrix_translate(Ent.EntMatx, vector3d(0, -1, 0));
				self->action = movement;
			}
			if (keys[SDL_SCANCODE_DOWN]){
				self->state = ES_Running;
				self->dr = Down;
				dy = -1;
				//sword_displacement(self, vector3d(0, -self->movementspeed * framechange, 0));
				self->action = movement;
			}
			//slog("movemnt x:%f y:%f z:%f frame:%f",dx ,dy ,dz, framechange );
			sword_displacement(self, vector3d((dx*(self->movementspeed * framechange)),( dy*(self->movementspeed * framechange)), dz));

		}
		if (keys[SDL_SCANCODE_D]){
			self->action = attack;
		}
		self->think(self);
	}
	while (SDL_PollEvent(&event))
	{
		switch (event.type){
			//player event
		case SDL_KEYUP:
			//self->action = none;
			dx = 0;
			dy = 0;
			dz = 0;
			if (self->state == ES_Running){
				self->state = ES_Idle;
				self->action = none;
				prev_action = none;
				//update_model(self);
				self->update_model(self);
			}
			switch (event.key.keysym.sym)
			{
			case SDLK_d: self->action = none; break;
			default:
				break;
			}
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_c: self->attack(self); break;
			default:
				break;
			}
		default:
			break;

		}
	}
}

/*eol@eof*/