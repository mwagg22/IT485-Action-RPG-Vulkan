#include <stdlib.h>
#include <string.h>
#include<math.h>
#include "simple_logger.h"
#include "g_control.h"
#include "g_entity.h"
#include "g_floor.h"
#include "g_swordhero.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"
#include "g_hud.h"
#include"g_text.h"
Entity *other;
glob_model_pool *pool;
int attacknum=0;
int specialnum = 0;
Vector3D bboxmin, bboxmax;
bool up_trigger = false;
bool down_trigger = false;
Actioninput prev_action=none;
float framechange;
SDL_Event event;
float dx = 0.0;
float dy = 0.0;
float dz = 0.0;
float upx = 0;
float upy = 0;
float upz = 0;
Matrix4 mtxcopy_s;

void sword_think(Entity *self){
	EntityState prev_state = self->state;
	switch (self->action){
	case attack:
		if (self->can_attack == true){
			//slog("self->attacknum b4:%i", self->attacknum);
			sword_attack(self);
			//self->attacknum++;
			//slog("self->attacknum af:%i", self->attacknum);
			prev_action = self->action;
		}
		break;
	case special:
		if (self->can_special == true){
			//slog("special b4:%i", specialnum);
			sword_special(self);
			//self->attacknum++;
			//slog("self->attacknum af:%i", specialnum);
			prev_action = self->action;
		}
		break;
	case movement:
		if (self->action != prev_action){
			self->update_model(self);
			prev_action = self->action;
		}
		break;
	case block:
		if (self->can_block==true){
			sword_block(self);
			//self->update_model(self);
			prev_action = self->action;
		}
		break;
	default:
		break;
	}
	//self->action = none;
}
void sword_attack(Entity *self){
	switch (self->attacknum){
		//slog("attack num: %i", self->attacknum);
	case 0:
		if (self->can_attack == true){
			self->can_block = false;
			if (up_trigger){
				self->attacknum = 3;
				self->can_attack = false;
				self->state = ES_Attacking;
				//update_model(self);
				self->update_model(self);
			}
			else if (down_trigger){
				self->attacknum = 5;
				self->can_attack = false;
				self->state = ES_Attacking;
				//update_model(self);
				self->update_model(self);
			}
			else{
				//slog("attack num1: %i", self->attacknum);
				self->can_attack = false;
				self->state = ES_Attacking;
				//update_model(self);
				self->update_model(self);
				//self->attacknum++;
			}
		}
		break;
	case 1:
		if (self->can_attack == true){
			self->can_block = false;
		if (up_trigger){
			self->attacknum = 3;
			self->can_attack = false;
			self->state = ES_Attacking;
			//update_model(self);
			self->update_model(self);
		}
		else if (down_trigger){
			self->attacknum = 5;
			self->can_attack = false;
			self->state = ES_Attacking;
			//update_model(self);
			self->update_model(self);
		}
		else{
			//if(self->can_attack == true){
			//slog("attack num2: %i", self->attacknum);
			self->state = ES_Attacking;
			//update_model(self);
			self->update_model(self);
			self->can_attack = false;
			//self->attacknum++;
		}
		}
	case 2:
		if (self->can_attack == true){
			self->can_block = false;
			//slog("attack num3: %i", self->attacknum);
			self->state = ES_Attacking;
			//update_model(self);
			self->update_model(self);
			self->can_attack = false;
			//self->attacknum = 0;
		}
		break;
	case 3:
		if (self->can_attack == true){
			self->can_block = false;
			//slog("attack num3: %i", self->attacknum);
			self->state = ES_Attacking;
			//update_model(self);
			self->update_model(self);
			self->can_attack = false;
			//self->attacknum = 0;
		}
		break;
	case 4:
		if (self->can_attack == true){
			self->can_block = false;
			//slog("attack num3: %i", self->attacknum);
			self->state = ES_Attacking;
			//update_model(self);
			self->update_model(self);
			self->can_attack = false;
			//self->attacknum = 0;
		}
		break;
	default:
		//slog("How i get here");
		break;

	}
}
void sword_special(Entity *self){
	if (self->can_special == true){
		self->can_block = false;
		if (up_trigger || (self->controling == 0 && self->specialnum == 2)){
			self->specialnum = 2;
			self->state = ES_Special;
			self->can_attack = false;
			self->can_special = false;
			//update_model(self);
			self->update_model(self);
		}
		else if (down_trigger || (self->controling == 0 && self->specialnum == 1)){
			self->specialnum = 1;
			self->state = ES_Special;
			self->can_attack = false;
			self->can_special = false;
			//update_model(self);
			self->update_model(self);
		}
		else{
			self->specialnum = 0;
			self->state = ES_Special;
			self->can_attack = false;
			self->can_special = false;
			//update_model(self);
			self->update_model(self);
		}
	}

}
void update_sword_model(Entity *self){
	self->frame = 0;
	if (self->state == ES_Idle){
		 0;
		 self->model = self->mods.idle;//gf3d_model_load_animated("//player//sword//sword_idle//sword_s1_idle", "sword_s1", 0, 29);
		self->can_attack = true;
		self->can_block = true;
	}
	if (self->state == ES_Running){
		 1;
		 self->model = self->mods.run;//gf3d_model_load_animated("//player//sword//sword_run//sword_s1_run", "sword_s1", 0, 29);
	}
	if (self->state == ES_Blocking){
		 2;
		 self->model = self->mods.block;// gf3d_model_load_animated("//player//sword//sword_block//sword_block", "sword_s1", 0, 29);
		self->can_attack = false;
	}
	if (self->state == ES_Hit){
		 3;
		 self->model = self->mods.hit;//gf3d_model_load_animated("//player//sword//hit//sword_hit1", "sword_s1", 0, 30);
		self->can_attack = false;
	}
	if (self->state == ES_Attacking){
		if (self->attacknum == 0){
			 4;
			//slog("attack1");
			 self->model = self->mods.attack1;//gf3d_model_load_animated("//player//sword//sword_attack1//sword_s1_a1", "sword_s1", 0, 29);
		//create_hitbox(self, other, self->Hitbox.m_vecMin, self->Hitbox.m_vecMax, self->up);
		self->attacknum=1;
		}
		
		else if(self->attacknum==1){
			 5;
			//slog("attack2");
			 self->model = self->mods.attack2;//gf3d_model_load_animated("//player//sword//sword_attack2//sword_s1_a2", "sword_s1", 0, 29);
		//create_hitbox(self, other, self->Hitbox.m_vecMin, self->Hitbox.m_vecMax, self->up);
		self->attacknum=2;
		}
		else if(self->attacknum==2){
			 6;
			//slog("attack3");
			 self->model = self->mods.attack3;// gf3d_model_load_animated("//player//sword//sword_attack3//sword_s1_a3", "sword_s1", 0, 29);
		//create_hitbox(self, other, self->Hitbox.m_vecMin, self->Hitbox.m_vecMax, self->up);
		self->attacknum = 0;
		}
		else if (self->attacknum == 3){
			 7;
			//slog("attack3");
			 self->model = self->mods.attack4;// gf3d_model_load_animated("//player//sword//sword_attack2//sword_s1_a2", "sword_s1", 0, 29);
			//create_hitbox(self, other, self->Hitbox.m_vecMin, self->Hitbox.m_vecMax, self->up);
			self->attacknum = 4;
		}
		else if (self->attacknum == 4){
			 8;
			//slog("attack3");
			 self->model = self->mods.attack5;// gf3d_model_load_animated("//player//sword//sword_u2//sword_u2", "sword_s1", 0, 29);
			//create_hitbox(self, other, self->Hitbox.m_vecMin, self->Hitbox.m_vecMax, self->up);
			self->attacknum = 5;
		}
		else if (self->attacknum == 5){
			 9;
			//slog("attack3");
			 self->model = self->mods.attack6;//gf3d_model_load_animated("//player//sword//sword_d1//sword_d1", "sword_s1", 0, 29);
			//create_hitbox(self, other, self->Hitbox.m_vecMin, self->Hitbox.m_vecMax, self->up);
			//self->attacknum = 5;
		}
	}
	else if (self->state == ES_Special){
		self->attacknum = 0;
		if (self->specialnum == 0){
			 10;
			//slog("sp1");
			 self->model = self->mods.special1;//gf3d_model_load_animated("//player//sword//sword_attack1//sword_s1_a1", "sword_s1", 0, 29);
			//create_hitbox(self, other, self->Hitbox.m_vecMin, self->Hitbox.m_vecMin, self->up);
			//specialnum = 0;
		}

		else if (self->specialnum == 1){
			 11;
			//slog("sp2");
			 self->model = self->mods.special2;// gf3d_model_load_animated("//player//sword//s2//sp2", "stahn", 0, 80);
			//create_hitbox(self, other, self->Hitbox.m_vecMin, self->Hitbox.m_vecMin, self->up);
			//specialnum = 0;
		}
		else if (self->specialnum == 2){
			 12;
			//slog("sp3");
			 self->model = self->mods.special3;// gf3d_model_load_animated("//player//sword//s3//sp1", "stahn", 0, 24);
			//create_hitbox(self, other, self->Hitbox.m_vecMin, self->Hitbox.m_vecMin, self->up);
			//specialnum = 0;
		}
	}
}
void sword_displacement(Entity *self, Vector3D disp){
	//vector3d_normalize(&disp);
	vector3d_rotate_about_z(&disp, self->rotated);
	float angle;
	if (disp.x == 0 && disp.y == 0 && disp.z == 0){
		////slog("framecnt :%i",self->model->frameCount);
		return;
	}
	else{
		////slog("rotaiton");
		////slog("movemnt x:%f y:%f z:%f", disp.x, disp.y, disp.z);
		angle = vector3d_angle_between_vectors(self->up, disp);
		////slog("angle %f", angle);
		////slog("up before x:%f y:%f z:%f frame:%f", self->up.x, self->up.y, self->up.z, framechange);
		gfc_matrix_rotate(self->EntMatx, self->EntMatx, angle, vector3d(0, 0, 1));
		gfc_matrix_translate(self->EntMatx, disp);
		self->up.x = disp.x;
		self->up.y = disp.y;
		self->up.z = disp.z;
		upx = disp.x;
		upy = disp.y;
		upz = disp.z;
		gfc_matrix_copy(mtxcopy_s, self->EntMatx);
		//slog("position x:%f y:%f z:%f", self->position.x, self->position.y, self->position.z);
		////slog("up after x:%f y:%f z:%f frame:%f", self->up.x, self->up.y, self->up.z, framechange);
	}
}
void update_sword_ent(Entity *self){
	if (self->overworld == true){
		self->EntMatx[3][2] = return_terrain_height(&other[6], -self->position.x, self->position.y);
	}
	if (self->attacknum>5)self->attacknum = 0;
	set_position(self, self->EntMatx);
	if (self->controling == 0){
		sword_ai_think(self);
	}
	self->frame = self->frame + 0.9;
	if (self->state == ES_Attacking){
		if (self->attacknum == 0 && round(self->frame)==13){
			create_projectile_e(self, NULL, pool->hitbox, 5.0, 20, false, 1, vector3d(0, 0, 0));
		}
		if (self->attacknum == 1 && round(self->frame) == 14){
			create_projectile_e(self, NULL, pool->hitbox, 5.0, 20, false, 1, vector3d(0, 0, 0));
		}
		if (self->attacknum == 2 && round(self->frame) == 14){
			create_projectile_e(self, NULL, pool->hitbox, 5.0, 20, false, 1, vector3d(0, 0, 0));
		}
		if (self->attacknum == 3 && round(self->frame) == 14){
			create_projectile_e(self, NULL, pool->hitbox, 5.0, 20, false, 1, vector3d(0, 0, 0));
		}
		if (self->attacknum == 4 && round(self->frame) == 17){
			create_projectile_e(self, NULL, pool->hitbox, 5.0, 20, false, 1, vector3d(0, 0, 0));
		}
		if (self->attacknum == 5 && round(self->frame) == 17){
			create_projectile_e(self, NULL, pool->hitbox, 5.0, 20, false, 1, vector3d(0, 0, 0));
		}
	}
	if(self->frame>=20&&self->state==ES_Attacking){
		self->can_attack = true;
		self->can_block = true;
		self->can_special = true;
		////slog("can attack again self->attacknum:%i",self->attacknum);

	}

	if (self->state == ES_Special){
		if (self->specialnum == 0 && self->frame == 9){
		//create_mage_projectile(self, 3.0, 1.0);
		 create_projectile(self, 2, 5);
	}
	 if (self->specialnum == 1){
		 if (round(self->frame) == 15){
			 create_projectile_e(self, NULL, pool->hitbox, 5.0, 20, false, 1, vector3d(0, 0, 0));
		 }
		 if (round(self->frame) == 28){
			 create_projectile_e(self, NULL, pool->hitbox, 5.0, 20, false, 1, vector3d(0, 0, 0));
		 }
		 if (round(self->frame) == 34){
			 create_projectile_e(self, NULL, pool->hitbox, 5.0, 20, false, 1, vector3d(0, 0, 0));
		 }
		 if (round(self->frame) == 40){
			 create_projectile_e(self, NULL, pool->hitbox, 5.0, 20, false, 1, vector3d(0, 0, 0));
		 }
		 if (round(self->frame) == 46){
			 create_projectile_e(self, NULL, pool->hitbox, 5.0, 20, false, 1, vector3d(0, 0, 0));
		 }
		 if (round(self->frame) == 51){
			 create_projectile_e(self, NULL, pool->hitbox, 5.0, 20, false, 1, vector3d(0, 0, 0));
		 }
		 if (round(self->frame) == 57){
			 create_projectile_e(self, NULL, pool->hitbox, 5.0, 20, false, 1, vector3d(0, 0, 0));
		 }
		 if (round(self->frame) == 66){
			 create_projectile_e(self, NULL, pool->hitbox, 5.0, 20, false, 1, vector3d(0, 0, 0));
		 }
	 }
	 if (self->specialnum == 2){
		 if (round(self->frame) == 15){
			 create_projectile_e(self, NULL, pool->hitbox, 5.0, 20, false, 1, vector3d(0, 0, 0));
		 }
	 }
	}
	if (self->frame >= self->model->frameCount){
		if (self->controling == 0){
			up_trigger = false;
			down_trigger = false;
		}
	 if (self->state == ES_Hit){
		self->is_hit = false;
		self->in_action = false;
		self->state = ES_Idle;
		self->attacknum = 0;
		self->frame = 0;
		self->update_model(self);
	}
	 else if (self->state == ES_Attacking){
		 self->is_hit = false;
		 self->can_special = true;
		 self->can_block = true;
		 self->can_attack = true;
		 self->in_action = false;
		 self->state = ES_Idle;
		 self->attacknum = 0;
		 self->frame = 0;
		 self->update_model(self);
	 }
	 else if (self->state == ES_Special){
		 self->is_hit = false;
		 self->can_special = true;
		 self->can_block = true;
		 self->can_attack = true;	
		 self->in_action = false;
		 //slog("end sp");
		 self->specialnum = 0;
		 self->frame = 0;
		 self->action = none;
		 self->state = ES_Idle;
		 self->update_model(self);
	 }
	  else{
		  if (self->controling == 0){
			  up_trigger = false;
			  down_trigger = false;
		  }
		  self->is_hit = false;
		  self->in_action = false;
		 self->frame = 0;
	 }
		////slog("frame hit 0. state num:%i",self->state);
	}
	////slog("Frame %f", self->frame);
	
}

void sword_block(Entity *self){
	if (self->can_block == true){
		self->state = ES_Blocking;
		self->attacknum = 0;
		//slog("state: blocking");
		self->update_model(self);
	}


}
void init_sword_ent(Entity *self, int ctr, Entity *ents, glob_model_pool *pools){
	self->state = ES_Idle;
	self->dr = Up;
	self->attack=sword_attack;
	self->block = sword_block;
	self->update_model=update_sword_model;
	self->update_ent=update_sword_ent;
	self->think = sword_think;
	self->can_attack = true;
	self->can_hpskill = true;
	self->can_special = true;
	self->can_block = true;
	self->is_hit = false;
	self->action = none;
	self->prev_action = none;
	self->get_inputs = sword_get_inputs;
	self->up = vector3d(0, 1,0);
	//self->right = vector3d(0, 1, 0);
	self->controling = ctr;
	self->movementspeed = .50f;
	self->rotated = 0.0f;
	self->specialnum = 0;
	self->type = ES_Player;
	self->health = 400;
	self->attackdmg = 12;
	self->overworld = 1;
	other = ents;
	self->show = 1;
	//text_box *b = ;
	self->health_bar = create_healthbar("//other//ui//face//sword//sword_face", "//other//ui//sword_face", 12, 18, -7, &other[0]);
	self->health_bar->box = create_textbox("400", 14, 14.5, -6,NULL, true, &other[0]);
	self->health_bar->mp_box = create_textbox("4000", 14, 14.5, -8, NULL, true, &other[0]);
	gf3d_set_hitbox(self, vector3d(-5, -5, 0), vector3d(5, 5, 10));
	
	pool = pools;
	self->mods.idle = gf3d_model_load_animated("//player//sword//sword_idle//sword_s1_idle", "sword_s1", 0, 29);

	self->mods.run = gf3d_model_load_animated("//player//sword//sword_run//sword_s1_run", "sword_s1", 0, 29);

	self->mods.block = gf3d_model_load_animated("//player//sword//sword_block//sword_block", "sword_s1", 0, 29);

	self->mods.hit = gf3d_model_load_animated("//player//sword//hit//sword_hit1", "sword_s1", 0, 30);

	self->mods.attack1 = gf3d_model_load_animated("//player//sword//sword_attack1//sword_s1_a1", "sword_s1", 0, 29);

	self->mods.attack2 = gf3d_model_load_animated("//player//sword//sword_attack2//sword_s1_a2", "sword_s1", 0, 29);

	self->mods.attack3 = gf3d_model_load_animated("//player//sword//sword_attack3//sword_s1_a3", "sword_s1", 0, 29);

	self->mods.attack4 = gf3d_model_load_animated("//player//sword//sword_attack2//sword_s1_a2", "sword_s1", 0, 29);

	self->mods.attack5 = gf3d_model_load_animated("//player//sword//sword_u2//sword_u2", "sword_s1", 0, 29);

	self->mods.attack6 = gf3d_model_load_animated("//player//sword//sword_d1//sword_d1", "sword_s1", 0, 29);

	self->mods.special1 = gf3d_model_load_animated("//player//sword//sword_attack1//sword_s1_a1", "sword_s1", 0, 29);

	self->mods.special2 = gf3d_model_load_animated("//player//sword//s2//sp2", "stahn", 0, 80);

	self->mods.special3 = gf3d_model_load_animated("//player//sword//s3//sp1", "stahn", 0, 24);
	self->update_model(self);
	self->model->mesh[0]->minv.x += 3;
	gf3d_set_boundbox(self, self->model->mesh[0]->minv, self->model->mesh[0]->maxv);
	//other = ents;
}



void sword_get_inputs(Entity *self, const Uint8 *keys, float delta){
	framechange = delta;

	EntityState prev_state = self->state;
	self->prev_action = self->action;
	if (self->controling == 1){
		if (self->state != ES_Attacking&&self->state != ES_Blocking&&self->state != ES_Hit&&self->state != ES_Special){
			if (keys[SDL_SCANCODE_RIGHT]){
				self->state = ES_Running;
				//update_model(self);
				self->dr = Right;
				dx = -1.0;
				//sword_displacement(self, vector3d(-self->movementspeed * framechange, 0,0));
				self->action = movement;
			}
			if (keys[SDL_SCANCODE_LEFT]){
				self->state = ES_Running;
				//update_model(self);
				self->dr = Left;
				dx = 1.0;
				self->action = movement;
			}
			if (keys[SDL_SCANCODE_UP]){
				self->state = ES_Running;
				//update_model(self);
				self->dr = Up;
				dy = 1.0;
				self->action = movement;
			}
			if (keys[SDL_SCANCODE_DOWN]){
				self->state = ES_Running;
				self->dr = Down;
				dy = -1.0;
				self->action = movement;
			}
			sword_displacement(self, vector3d((dx*(self->movementspeed)), (dy*(self->movementspeed)), 0));

		}
		if (keys[SDL_SCANCODE_D]){
			self->action = attack;
		}
		if (keys[SDL_SCANCODE_F]){
			self->action = block;
		}
		if (keys[SDL_SCANCODE_S]){
			self->action = special;
		}
		self->think(self);
	}
	while (SDL_PollEvent(&event))
	{
		//if (event.type == SDL_MOUSEWHEEL)
			//	{
			//	if (event.wheel.y > 0) // scroll up
			//	{
			//		wheel -= 2.0;
			//	}
			//	else if (event.wheel.y < 0) // scroll down
			//	{
			//		wheel += 2.0;
			//	}
			//}
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
			case SDLK_f: self->action = none; self->state = ES_Idle; self->update_model(self); break;
			case SDLK_UP: up_trigger = false; break;
			case SDLK_DOWN: down_trigger = false; break;
			default:
				break;
			}
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_c: self->attack(self); break;
			case SDLK_r: other[1].controling = 1; self->controling = 0;  break;
			case SDLK_e: {
							 Vector3D d, e;
							 self->target = get_nearest_target(self, other);
							 vector3d_sub(d, self->position, self->target->position);
							 d.x = -d.x;
							 d.y = d.y;
							 d.z = 0;
							 e.x = upx;
							 e.y = upy;
							 e.z = upz;
							 vector3d_normalize(&d);
							 if (self->state != ES_Running){
								 rotate_towards_target(self, d, &e);
								 upx = e.x;
								 upy = e.y;
								 upz = e.z;
							 }}
				break;
			//case SDLK_g: break;
			case SDLK_UP: up_trigger = true; break;
			case SDLK_DOWN: down_trigger = true; break;
			default:
				break;
			}
		default:
			break;

		}
	}
}

void create_projectile(Entity *self, float speed, float dmg){
	Entity projEnt = *gf3d_entity_new();
	Model *proj = pool->wave;
	projEnt.model = proj;
	projEnt.up.x = upx; //self->up.x;
	projEnt.up.y = upy;//self->up.y;
	projEnt.up.z = upz;//self->up.z;
	////slog("up before x:%f y:%f z:%f ", self->up.x, self->up.y, self->up.z);
	//slog("up before x:%f y:%f z:%f ", projEnt.up.x, projEnt.up.y, projEnt.up.z);
	projEnt.movementspeed = speed;
	projEnt.attackdmg = dmg;
	projEnt.rotated = self->rotated;
	//projEnt.state = ES_Idle;
	projEnt.update_ent = update_ent;
	projEnt.controling = 0;
	//projEnt.get_inputs = get_proj_inputs;
	projEnt.type = ES_Projectile;
	projEnt.ProjectileData.Hitarray = (Entity*)gfc_allocate_array(sizeof(Entity), 10);
	projEnt.ProjectileData.destroyOncollision = true;
	gfc_matrix_identity(projEnt.EntMatx);
	gfc_matrix_copy(projEnt.EntMatx, mtxcopy_s);
	projEnt.parent = self;
	//gfc_scale_matrix(projEnt.EntMatx, 20, 20, 1);
	//gfc_matrix_copy(projEnt.EntMatx, self->EntMatx);
	//ents[3] = &projEnt;	
	gf3d_set_boundbox(&projEnt, projEnt.model->mesh[0]->minv, projEnt.model->mesh[0]->maxv);
	//
	spawn_Entity2(&projEnt);
}
void sword_ai_think(Entity *self){
	if (self->controling != 1){
		float attack_range = 5.00f;
		Entity *target = get_nearest_target(self, other);
		self->target = target;
		if (target){
			////slog("distance between targets :%f", round(vector3d_magnitude_between(target->position, self->position)));
			if (vector3d_magnitude_between(target->position, self->position) > attack_range){
				self->action = movement;
			}
			else if (vector3d_magnitude_between(target->position, self->position) <= attack_range){
				int rdm = (rand() % 30);
				if (rdm <= 18){
					self->action = attack;
					////slog("attack");
				}
				else if (rdm > 18){
					self->action = special;
					////slog("attack");
				}
				else{
					self->action = none;
					////slog("idle");
				}
			}

		}
		//EntityState prev_state = self->state;
		if (self->in_action == false){
			switch (self->action){
			case attack:
				if (self->can_attack == true){
					//rotate_towards_target(self, target);
					Vector3D d, e;
					vector3d_sub(d, self->position, target->position);
					d.x = -d.x;
					d.y = d.y;
					d.z = 0;
					e.x = upx;
					e.y = upy;
					e.z = upz;
					vector3d_normalize(&d);
					if (self->state = ES_Running){
						rotate_towards_target(self, d, &e);
						upx = e.x;
						upy = e.y;
						upz = e.z;
					}
					int rdma = (rand() % 30);
					if (rdma <= 10){
						up_trigger = true;
						//slog("sp2");
					}
					else if (rdma >= 20){
						down_trigger = true;
						//slog("sp1");
					}
					sword_attack(self);
					//turning = false;
				}
				break;
			case special:
				if (self->can_special == true){
					//rotate_towards_target(self, target);
					//slog("special");
					Vector3D d, e;
					vector3d_sub(d, self->position, target->position);
					d.x = -d.x;
					d.y = d.y;
					d.z = 0;
					e.x = upx;
					e.y = upy;
					e.z = upz;
					vector3d_normalize(&d);
					if (self->state = ES_Running){
						rotate_towards_target(self, d, &e);
						upx = e.x;
						upy = e.y;
						upz = e.z;
					}
					int rdms = (rand() % 30);
					if (rdms <= 10){
						self->specialnum = 2;
						//slog("sp2");
					}
					else if (rdms >= 20){
						self->specialnum = 1;
						//slog("sp1");
					}
					sword_special(self);
					//turning = false;
				}
				break;
			case movement:
				if (self->state != ES_Running){
					self->state = ES_Running;
					self->update_model(self);
				}
				Vector3D a, b, c;
				a.x = target->position.x;
				a.y = -target->position.y;
				a.z = 0;
				b.x = self->position.x;
				b.y = -self->position.y;
				b.z = 0;
				vector3d_sub(c, b, a);
				c.x = -round(c.x);
				c.y = -round(c.y);
				c.z = 0;
				vector3d_normalize(&c);
				if (self->state != ES_Attacking&&self->state != ES_Idle){
					sword_displacement(self, vector3d(c.x*.35, c.y*.35, 0));
				}
				break;
			case block:
				if (self->can_block == true){
					sword_block(self);
					self->in_action = true;
					//self->update_model(self);
				}
				break;
			case none:
				if (self->state != ES_Attacking&&self->state != ES_Special){
					self->state = ES_Idle;
					self->in_action = true;
					Vector3D d, e;
					vector3d_sub(d, self->position, target->position);
					d.x = -d.x;
					d.y = d.y;
					d.z = 0;
					e.x = upx;
					e.y = upy;
					e.z = upz;
					vector3d_normalize(&d);
					if (self->state != ES_Running){
						rotate_towards_target(self, d, &e);
						upx = e.x;
						upy = e.y;
						upz = e.z;
					}
					self->update_model(self);
				}
				break;
			default:
				break;
			}
		}
	}
}
/*eol@eof*/