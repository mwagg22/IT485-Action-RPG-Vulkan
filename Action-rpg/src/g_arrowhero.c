#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "simple_logger.h"
#include "g_control.h"
#include "g_entity.h"
#include "g_arrowhero.h"
#include "g_floor.h"
#include "g_hud.h"
#include"g_text.h"
Entity *other;
glob_model_pool *pool;
int attacknum_a=0;
Vector3D bboxmin, bboxmax;
bool up_trigger_arrow = false;
bool down_trigger_arrow = false;
bool held = false;
bool released = false;
bool roll_a = false;
Actioninput prev_action_a=none;
float framechange;
SDL_Event event;
float dx_a = 0;
float dy_a = 0;
float dz_a = 0;
float upx_a = 0;
float upy_a = 1;
float upz_a = 0;
Matrix4 mtxcopy;

void arrow_think(Entity *self){
	EntityState prev_state = self->state;

	switch (self->action){
	case attack:
		if (self->can_attack == true){
			//slog("attacknum_a b4:%i", attacknum_a);
			arrow_attack(self);
			//attacknum_a++;
			//slog("attacknum_a af:%i", attacknum_a);
			prev_action_a = self->action;
		}
		break;
	case special:
		if (self->can_special == true){
			slog("special b4:%i", self->specialnum);
			arrow_special(self);
			//attacknum++;
			slog("attacknum af:%i", self->specialnum);
			prev_action_a = self->action;
		}
		break;
	case movement:
		if (self->action != prev_action_a){
			self->update_model(self);
			prev_action_a = self->action;
		}
		break;
	case block:
		if (self->can_block==true){
			arrow_block(self);
			//self->update_model(self);
			prev_action_a = self->action;
		}
		break;
	default:
		break;
	}
	//self->action = none;
}

void arrow_attack(Entity *self){
	switch (attacknum_a){
		//slog("attack num: %i", attacknum_a);
	case 0:
		if (self->can_attack == true){
			self->in_action = true;
			self->can_attack = false;
			self->can_block = false;
			if (up_trigger_arrow){
				attacknum_a = 3;
				self->can_attack = false;
				self->state = ES_Attacking;
				//update_model(self);
				self->update_model(self);
			}
			else if (down_trigger_arrow){
				self->in_action = true;
				attacknum_a = 5;
				self->can_attack = false;
				self->state = ES_Attacking;
				//update_model(self);
				self->update_model(self);
			}
			else{
				self->in_action = true;
				//slog("attack num1: %i", attacknum_a);
				self->can_attack = false;
				self->state = ES_Attacking;
				//update_model(self);
				self->update_model(self);
				//attacknum_a++;
			}
		}
		break;
	case 1:
		if (self->can_attack == true){
			self->in_action = true;
			self->can_attack = false;
			self->can_block = false;
			if (up_trigger_arrow){
				attacknum_a = 3;
				self->can_attack = false;
				self->state = ES_Attacking;
				//update_model(self);
				self->update_model(self);
			}
			else if (down_trigger_arrow){
				self->in_action = true;
				attacknum_a = 5;
				self->can_attack = false;
				self->state = ES_Attacking;
				//update_model(self);
				self->update_model(self);
			}
			else{
				self->in_action = true;
				//slog("attack num1: %i", attacknum_a);
				self->can_attack = false;
				self->state = ES_Attacking;
				//update_model(self);
				self->update_model(self);
				//attacknum_a++;
			}
		}
		break;
	case 3:
		if (self->can_attack == true){
			self->in_action = true;
			self->can_attack = false;
			self->can_block = false;
			//slog("attack num3: %i", attacknum_a);
			self->state = ES_Attacking;
			//update_model(self);
			self->update_model(self);
			self->can_attack = false;
			//attacknum_a = 0;
		}
		break;
	default:
		slog("How i get here");
		break;

	}
}
void arrow_special(Entity *self){

		if (self->can_special == true){
			roll_a = false;
			self->can_block = false;
			if (up_trigger_arrow || (self->controling == 0 && self->specialnum == 2)){
				self->specialnum = 2;
				self->state = ES_Special;
				self->can_attack = false;
				self->can_special = false;
				//update_model(self);
				self->update_model(self);
			}
			else if (down_trigger_arrow || (self->controling == 0 && self->specialnum == 1)){
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
void arrow_displacement(Entity *self, Vector3D disp){
	//vector3d_normalize(&disp);
	vector3d_rotate_about_z(&disp, self->rotated);
	float angle;
	if (disp.x == 0 && disp.y == 0 && disp.z == 0){
		//slog("framecnt :%i",self->model->frameCount);
		return;
	}
	else{
		angle = vector3d_angle_between_vectors(self->up, disp);
		//slog("Angle:%f", angle);
		//slog("up before movement x:%f y:%f z:%f moved", self->up.x, self->up.y, self->up.z);
		if (!self->is_hit&&!self->collision){
			gfc_matrix_rotate(self->EntMatx, self->EntMatx, angle, vector3d(0, 0, 1));
			self->up.x = disp.x;
			self->up.y = disp.y;
			self->up.z = disp.z;
			upx_a = disp.x;
			upy_a = disp.y;
			upz_a = disp.z;
		}
		gfc_matrix_translate(self->EntMatx, disp);
		gfc_matrix_copy(mtxcopy, self->EntMatx);
		//slog("up after x:%f y:%f z:%f frame:%f", self->up.x, self->up.y, self->up.z, framechange);
	}
	//
}
void update_arrow_model(Entity *self){
	self->frame = 0;
	if (self->state == ES_Idle){
		self->model = self->mods.idle;
		self->can_attack = true;
		self->can_block = true;
		//self->in_action = false;
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
		if (released == true&&attacknum_a==1){
			self->model = self->mods.attack1;
			create_arrow_projectile(self, 3.0, 1.0);
			released = false;
			attacknum_a = 0;		
		}
		else if (attacknum_a == 0){
			//slog("attack1");
			self->model = self->mods.attack2;
		attacknum_a = 1;
		//create_hitbox(self, vector3d(-5,-5, 0), vector3d(5, 0, 3), vector3d(0, 0, 0));
		}
		else if (attacknum_a == 3){
			//slog("attack3");
			roll_a = true;
			self->model = self->mods.attack3;
			held = false;
			attacknum_a = 3;
		}
		else if (attacknum_a == 5){
			//slog("attack3");
			self->model = self->mods.attack4;
			//create_hitbox(self, vector3d(0, -5, 0), vector3d(5, 5, 3), vector3d(0, 0, 0));
			attacknum_a = 5;
		}
	}
	else if (self->state == ES_Special){
		if (self->cast == 0){
			if (self->specialnum == 0){
				slog("sp1");
				self->model = self->mods.special1;
				//create_hitbox(self, other, self->Hitbox.m_vecMin, self->Hitbox.m_vecMin, self->up);
				//specialnum = 0;
			}

			else if (self->specialnum == 1){
				slog("sp2");
				self->model = self->mods.special2;
				//create_hitbox(self, other, self->Hitbox.m_vecMin, self->Hitbox.m_vecMin, self->up);
				//specialnum = 0;
			}
			else if (self->specialnum == 2){
				slog("sp3");
				self->model = self->mods.special3;
				//create_hitbox(self, other, self->Hitbox.m_vecMin, self->Hitbox.m_vecMin, self->up);
				//specialnum = 0;
			}
		}
	}
}

void update_arrow_ent(Entity *self){
	//self->EntMatx[3][2] = return_terrain_height(&other[6], -self->position.x, self->position.y);
	set_position(self, self->EntMatx);
	gfc_matrix_copy(mtxcopy, self->EntMatx);
	if (self->overworld == false){
		if (self->position.x<-30){
			self->EntMatx[3][0] = self->EntMatx[3][0] + 1;
		}
		if (self->position.x>30){
			self->EntMatx[3][0] = self->EntMatx[3][0] - 1;
		}
		if (self->position.y<-30){
			self->EntMatx[3][1] = self->EntMatx[3][1] + 1;
		}
		if (self->position.y>30){
			self->EntMatx[3][1] = self->EntMatx[3][1] - 1;
		}
	}
	if (self->controling == 0){
		arrow_ai_think(self);
	}
	if (roll_a){
		arrow_displacement(self, vector3d(self->up.x, self->up.y, 0));
	}
	self->frame = self->frame + 0.9;
	if (self->frame >= 25 && self->state == ES_Attacking&&held == false && roll_a==false){
		//self->can_attack = true;
		self->can_block = true;
		//slog("can attack again attacknum_a:%i",attacknum_a);


	}
	if (self->state == ES_Attacking){
		if (attacknum_a == 5 && round(self->frame) == 17){
			create_projectile_e(self, NULL, pool->hitbox, 5.0, 20, false, 1, vector3d(0, 0, 0));
		}
	}
	if (self->state == ES_Special){
		if (self->specialnum == 0 && round(self->frame) == 32){
			create_projectile_e(self, self->target, pool->dragon, 5.0, 20, 4, 0, vector3d(0, 0, 0));
		}
		if (self->specialnum == 1){
			//create_mage_projectile(self, 3.0, 1.0);
			if (round(self->frame) == 32){
				create_arrow_projectile(self, 3.0, 1.0);
				
			}
			if (round(self->frame) == 37){
				create_arrow_projectile(self, 3.0, 1.0);
			}
			if (round(self->frame) == 40){
				create_arrow_projectile(self, 3.0, 1.0);
			}
		}
		if (self->specialnum == 2 && round(self->frame) == 57){
			self->target=get_nearest_target(self, other);
			create_projectile_e(self, self->target, pool->thunder, 5.0, 20, 1, 0, vector3d(0, 0, -1.9));
		}
	}
	if (self->frame >= self->model->frameCount-1){
		if (held == true&&attacknum_a==1&&self->controling==1){
			self->can_block = false;
			self->can_attack = false;
			released = false;
			self->frame = 30;
		}
		else if (self->state==ES_Attacking){
			if (self->controling == 0){
				//slog("here");
				self->state = ES_Attacking;
				released = true;
				attacknum_a = 1;
				self->update_model(self);
			}
			released = false;
				roll_a = false;
				//self->in_action = false;
				self->state = ES_Idle;
				attacknum_a = 0;
				self->frame = 0;
				self->update_model(self);
			}
		else if (self->state == ES_Hit){
			self->is_hit = false;
			roll_a = false;
			self->in_action = false;
			up_trigger_arrow = false;
			down_trigger_arrow = false;
			self->state = ES_Idle;
			attacknum_a = 0;
			self->frame = 0;
			self->update_model(self);
		}
		else if (self->state == ES_Special){
			self->is_hit = false;
			self->can_special = true;
			self->can_block = true;
			self->can_attack = true;
			self->in_action = false;
			slog("end sp");
			self->specialnum = 0;
			self->frame = 0;
			self->action = none;
			self->state = ES_Idle;
			self->update_model(self);
		}
		else{
			released = true;
			up_trigger_arrow = false;
			down_trigger_arrow = false;
			self->frame = 0;
			self->in_action = false;
		}	
		//slog("frame hit 0. state num:%i",self->state);
	}
	//slog("Frame %f", self->frame);
	
}

void arrow_block(Entity *self){
	if (self->can_block == true){
		self->state = ES_Blocking;
		attacknum_a = 0;
		//slog("state: blocking");
		self->update_model(self);
	}


}
void init_arrow_ent(Entity *self, int ctr, Entity *ents, glob_model_pool *pools){
	self->state = ES_Idle;
	self->dr = Up;
	self->attack=arrow_attack;
	self->block = arrow_block;
	self->update_model=update_arrow_model;
	self->update_ent=update_arrow_ent;
	self->think = arrow_think;
	self->can_attack = true;
	self->can_hpskill = true;
	self->can_special = true;
	self->can_block = true;
	self->is_hit = false;
	self->action = none;
	self->prev_action = none;
	self->get_inputs = arrow_get_inputs;
	self->show = 0;
	self->overworld = 0;
	self->up = vector3d(0, 1,0);
	self->health = 400;
	self->attackdmg = 12;
	//self->right = vector3d(0, 1, 0);
	self->controling = ctr;
	self->movementspeed = 1.0f;
	self->rotated = 0.0f;
	self->health_bar = create_healthbar("//other//ui//face//sword//sword_face", "//other//ui//arrow_face", 12, 10, -7, &other[0]);
	self->health_bar->box = create_textbox("300", 14, 7, -6, NULL, true, &other[0]);
	self->health_bar->mp_box = create_textbox("4000", 14, 7, -8, NULL, true, &other[0]);
	self->type = ES_Player;
	pool = pools;
	other = ents;
	self->mods.idle = self->model = gf3d_model_load_animated("//player//bow//idle//bow_idle", "arrow_s1", 0, 30);

	self->mods.run = gf3d_model_load_animated("//player//bow//run//bow_run", "arrow_s1", 0, 26);

	self->mods.block = gf3d_model_load_animated("//player//bow//block//arrow_block", "arrow_s1", 0, 55);

	self->mods.hit = gf3d_model_load_animated("//player//bow//hit//arrow_hit", "arrow_s1", 0, 24);

	self->mods.attack1 = gf3d_model_load_animated("//player//bow//release//bow_shoot", "arrow_s1", 0, 22);

	self->mods.attack2 = gf3d_model_load_animated("//player//bow//charge//bow_hold", "arrow_s1", 0, 35);

	self->mods.attack3 = gf3d_model_load_animated("//player//bow//roll//bow_roll", "arrow_s1", 0, 28);

	self->mods.attack4 = gf3d_model_load_animated("//player//bow//kick//bow_kick", "arrow_s1", 0, 32);

	self->mods.special1 = gf3d_model_load_animated("//player//bow//sp1//sp", "arrow_s1", 0, 59);

	self->mods.special2 = gf3d_model_load_animated("//player//bow//sp2//sp", "arrow_s1", 0, 67);

	self->mods.special3 = gf3d_model_load_animated("//player//bow//sp3//sp3", "arrow_s1", 0, 75);
	self->update_model(self);
	self->model->mesh[0]->minv.x += 1;
	gf3d_set_boundbox(self, self->model->mesh[0]->minv, self->model->mesh[0]->maxv);
	//other = ents;
}



void arrow_get_inputs(Entity *self, const Uint8 *keys, float delta){
	framechange = delta;

	EntityState prev_state = self->state;
	self->prev_action = self->action;
	if (self->controling == 1){
		self->in_action = false;
		if (self->state != ES_Attacking&&self->state != ES_Blocking&&self->state != ES_Hit&&self->state != ES_Special){
			if (keys[SDL_SCANCODE_RIGHT]){
				self->state = ES_Running;
				//update_model(self);
				self->dr = Right;
				dx_a = -1;
				//arrow_displacement(self, vector3d(-self->movementspeed * framechange, 0,0));
				self->action = movement;
			}
			if (keys[SDL_SCANCODE_LEFT]){
				self->state = ES_Running;
				//update_model(self);
				self->dr = Left;
				dx_a = 1;
				self->action = movement;
			}
			if (keys[SDL_SCANCODE_UP]){
				self->state = ES_Running;
				//update_model(self);
				self->dr = Up;
				dy_a = 1;
				self->action = movement;
			}
			if (keys[SDL_SCANCODE_DOWN]){
				self->state = ES_Running;
				self->dr = Down;
				dy_a = -1;
				self->action = movement;
			}
			//slog("movemnt x:%f y:%f z:%f frame:%f",dx_a ,dy_a ,dz_a, framechange );
			arrow_displacement(self, vector3d((dx_a*(self->movementspeed)), (dy_a*(self->movementspeed)), dz_a));

		}
		if (keys[SDL_SCANCODE_D]){
			held = true;
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
		switch (event.type){
			//player event
		case SDL_KEYUP:
			//self->action = none;
			dx_a = 0;
			dy_a = 0;
			dz_a = 0;
			if (self->state == ES_Running){
				self->state = ES_Idle;
				self->action = none;
				prev_action_a = none;
				//update_model(self);
				self->update_model(self);
			}
			switch (event.key.keysym.sym)
			{
			case SDLK_d: self->action = none; held = false; released = true; if (self->state == ES_Attacking&&attacknum_a == 1){ self->update_model(self); } break;
			case SDLK_f: self->action = none; self->state = ES_Idle; self->update_model(self); break;
			case SDLK_UP: up_trigger_arrow = false; break;
			case SDLK_DOWN: down_trigger_arrow = false; break;
			default:
				break;
			}
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_c: self->attack(self); break;
			case SDLK_r:other[2].controling = 1; self->controling = 0;  break;
			case SDLK_e:
			{
						   Vector3D d, e;
						   self->target = get_nearest_target(self, other);
						   vector3d_sub(d, self->position, self->target->position);
						   d.x = -d.x;
						   d.y = d.y;
						   d.z = 0;
						   e.x = upx_a;
						   e.y = upy_a;
						   e.z = upz_a;
						   vector3d_normalize(&d);
						   if (self->state != ES_Running){
							   rotate_towards_target(self, d, &e);
							   upx_a = e.x;
							   upy_a = e.y;
							   upz_a = e.z;
						   }}
				break;
			case SDLK_g: create_arrow_projectile(&self, 3.0, 1.0); break;
			case SDLK_UP: up_trigger_arrow = true; break;
			case SDLK_DOWN: down_trigger_arrow = true; break;
			default:
				break;
			}
		default:
			break;

		}
	}
}


void create_arrow_projectile(Entity *self, float speed, float dmg){
	Entity projEnt = *gf3d_entity_new();
	Model *proj = pool->arrow;
	projEnt.model = proj;
	projEnt.up.x = upx_a;
	projEnt.up.y = upy_a;
	projEnt.up.z = upz_a;
	projEnt.movementspeed = speed;
	projEnt.attackdmg = dmg;
	projEnt.rotated = self->rotated;
	projEnt.state = ES_Idle;
	projEnt.update_ent = update_ent;
	projEnt.controling = 0;
	//projEnt.get_inputs = get_proj_inputs;
	projEnt.type = ES_Projectile;
	projEnt.parent = self;
	projEnt.ProjectileData.Hitarray = (Entity*)gfc_allocate_array(sizeof(Entity), 10);
	projEnt.ProjectileData.destroyOncollision = true;
	gfc_matrix_identity(projEnt.EntMatx);
	gfc_matrix_copy(projEnt.EntMatx, mtxcopy);
	//gfc_matrix_copy(projEnt.EntMatx, self->EntMatx);
	//ents[3] = &projEnt;	
	gf3d_set_boundbox(&projEnt, projEnt.model->mesh[0]->minv, projEnt.model->mesh[0]->maxv);
	//
	spawn_Entity2(&projEnt);
}
void arrow_ai_think(Entity *self){	
	 if(self->controling != 1){		
		float attack_range = 20.00f;
		Entity *target = get_nearest_target(self, return_game_list());
		if (target){
			//slog("distance between targets :%f", round(vector3d_magnitude_between(target->position, self->position)));
			if (vector3d_magnitude_between(target->position, self->position) < attack_range &&self->state!=ES_Special){
					self->action = movement;
			}
			else if (vector3d_magnitude_between(target->position, self->position) >= attack_range){
				int rdm = (rand() % 30);
				if (rdm <= 18){
					self->action = attack;
					//slog("attack");
				}
				else if (rdm > 18){
					self->action = special;
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
					//rotate_towards_target(self, target);
					Vector3D d,e;
					vector3d_sub(d, self->position, target->position);
					d.x = -d.x;
					d.y = d.y;
					d.z = 0;
					e.x = upx_a;
					e.y = upy_a;
					e.z = upz_a;
					vector3d_normalize(&d);
					if (self->state = ES_Running){
						rotate_towards_target(self, d,&e);
						upx_a = e.x;
						upy_a = e.y;
						upz_a = e.z;
					}
					arrow_attack(self);
					//turning = false;
				}
				break;
			case special:
				if (self->can_special == true){
					//rotate_towards_target(self, target);
					slog("special");
					Vector3D d, e;
					vector3d_sub(d, self->position, target->position);
					d.x = -d.x;
					d.y = d.y;
					d.z = 0;
					e.x = upx_a;
					e.y = upy_a;
					e.z = upz_a;
					vector3d_normalize(&d);
					if (self->state == ES_Running){
						rotate_towards_target(self, d, &e);
						upx_a = e.x;
						upy_a = e.y;
						upz_a = e.z;
					}
					int rdms = (rand() % 30);
					if (rdms <= 7){
						self->specialnum = 2;
						slog("sp2");
					}
					else if (rdms >= 20){
						self->specialnum = 1;
						slog("sp1");
					}
					arrow_special(self);
					//turning = false;
				}
				break;
			case movement:
				if (self->state != ES_Running){
					self->state = ES_Running;
					self->update_model(self);
				}
				Vector3D a,b,c;
				a.x = target->position.x;
				a.y = -target->position.y;
				a.z = 0;
				b.x = self->position.x;
				b.y = -self->position.y;
				b.z = 0;
				vector3d_sub(c, b, a);
				c.x = round(c.x);
				c.y = round(c.y);
				c.z = 0;
				vector3d_normalize(&c);
				if (self->state != ES_Attacking&&self->state != ES_Idle){
					arrow_displacement(self, vector3d(c.x*.35, c.y*.35, 0));
				}
				break;
			case block:
				if (self->can_block == true){
					arrow_block(self);
					self->in_action = true;
					//self->update_model(self);
				}
				break;
			case none:
				if (self->state != ES_Attacking&&self->state != ES_Special){
					self->state = ES_Idle;
					self->in_action = true;
					Vector3D d,e;
					vector3d_sub(d, self->position, target->position);
					d.x = -d.x;
					d.y = d.y;
					d.z = 0;
					e.x = upx_a;
					e.y = upy_a;
					e.z = upz_a;
					vector3d_normalize(&d);
					if (self->state != ES_Running){
						rotate_towards_target(self, d,&e);
						upx_a = e.x;
						upy_a = e.y;
						upz_a = e.z;
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