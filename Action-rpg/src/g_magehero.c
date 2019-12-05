#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "simple_logger.h"
#include "g_control.h"
#include "g_entity.h"
#include "g_magehero.h"
#include "g_floor.h"
Entity *other;
glob_model_pool *pool;
int attacknum_m=0;
Vector3D bboxmin, bboxmax;
bool up_trigger_m = false;
bool down_trigger_m = false;
Actioninput prev_action_m=none;
float framechange;
SDL_Event event;
float dx_m = 0;
float dy_m = 0;
float dz_m = 0;
float upx_m = 0;
float upy_m = 0;
float upz_m = 0;
Matrix4 mtxcopy_m;

void mage_think(Entity *self){
	EntityState prev_state = self->state;
	
	switch (self->action){
	case attack:
		if (self->can_attack == true){
			//slog("attacknum_m b4:%i", attacknum_m);
			mage_attack(self);
			//attacknum_m++;
			//slog("attacknum_m af:%i", attacknum_m);
			prev_action_m = self->action;
		}
		break;
	case special:
		if (self->can_special == true){
			slog("special b4:%i", self->specialnum);
			mage_special(self);
			//attacknum++;
			slog("attacknum af:%i", self->specialnum);
			prev_action_m = self->action;
		}
		break;
	case movement:
		if (self->action != prev_action_m){
			self->update_model(self);
			prev_action_m = self->action;
		}
		break;
	case block:
		if (self->can_block==true){
			mage_block(self);
			//self->update_model(self);
			prev_action_m = self->action;
		}
		break;
	default:
		break;
	}
	//self->action = none;
}
void mage_attack(Entity *self){
	switch (attacknum_m){
		//slog("attack num: %i", attacknum_m);
	case 0:
		if (self->can_attack == true){
			self->in_action = true;
			self->can_block = false;
			if (up_trigger_m){
				self->in_action = true;
				attacknum_m = 3;
				self->can_attack = false;
				self->state = ES_Attacking;
				//update_model(self);
				self->update_model(self);
			}
			else if (down_trigger_m){
				self->in_action = true;
				attacknum_m = 5;
				self->can_attack = false;
				self->state = ES_Attacking;
				//update_model(self);
				self->update_model(self);
			}
			else{
				self->in_action = true;
				//slog("attack num1: %i", attacknum_m);
				self->can_attack = false;
				self->state = ES_Attacking;
				//update_model(self);
				self->update_model(self);
				//attacknum_m++;
			}
		}
		break;
	case 1:
		if (self->can_attack == true){
			self->in_action = true;
			self->can_block = false;
			if (up_trigger_m){
				self->in_action = true;
			attacknum_m = 3;
			self->can_attack = false;
			self->state = ES_Attacking;
			//update_model(self);
			self->update_model(self);
		}
			else if (down_trigger_m){
				self->in_action = true;
			attacknum_m = 5;
			self->can_attack = false;
			self->state = ES_Attacking;
			//update_model(self);
			self->update_model(self);
		}
			else{
				self->in_action = true;
			//if(self->can_attack == true){
			//slog("attack num2: %i", attacknum_m);
			self->state = ES_Attacking;
			//update_model(self);
			self->update_model(self);
			self->can_attack = false;
			//attacknum_m++;
		}
		}
	case 2:
		if (self->can_attack == true){
			self->in_action = true;
			self->can_block = false;
			//slog("attack num3: %i", attacknum_m);
			self->state = ES_Attacking;
			//update_model(self);
			self->update_model(self);
			self->can_attack = false;
			//attacknum_m = 0;
		}
		break;
	case 3:
		if (self->can_attack == true){
			self->in_action = true;
			self->can_block = false;
			//slog("attack num3: %i", attacknum_m);
			self->state = ES_Attacking;
			//update_model(self);
			self->update_model(self);
			self->can_attack = false;
			//attacknum_m = 0;
		}
		break;
	default:
		slog("How i get here");
		break;

	}
}
void mage_special(Entity *self){
		if (self->can_special == true){
			self->can_block = false;
			if (up_trigger_m || (self->controling == 0 && self->specialnum == 2)){
				self->specialnum = 2;
				self->state = ES_Special;
				self->can_attack = false;
				self->can_special = false;
				//update_model(self);
				self->update_model(self);
			}
			else if (down_trigger_m || (self->controling == 0 && self->specialnum == 1)){
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
void update_mage_model(Entity *self){
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
		if (attacknum_m == 0){
			//slog("attack1");
			self->model = self->mods.attack1;
		//create_hitbox(self, vector3d(-5,-5, 0), vector3d(5, 0, 3), vector3d(0, 0, 0));
		attacknum_m=0;
		}
		else if (attacknum_m == 3){
			//slog("attack3");
			self->model = self->mods.attack2;
			//create_hitbox(self, vector3d(0, -5, 0), vector3d(5, 5, 3), vector3d(0, 0, 0));
			attacknum_m = 3;
		}
		else if (attacknum_m == 5){
			//slog("attack3");
			self->model = self->mods.attack3;
			//create_hitbox(self, vector3d(0, -5, 0), vector3d(5, 5, 3), vector3d(0, 0, 0));
			attacknum_m = 5;
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
				self->model = self->mods.special1;
				//create_hitbox(self, other, self->Hitbox.m_vecMin, self->Hitbox.m_vecMin, self->up);
				//specialnum = 0;
			}
			else if (self->specialnum == 2){
				slog("sp3");
				self->model = self->mods.special1;
				//create_hitbox(self, other, self->Hitbox.m_vecMin, self->Hitbox.m_vecMin, self->up);
				//specialnum = 0;
			}
		}
		 if (self->cast == 1){
			 slog("execute");
			 self->model = self->mods.special2;
			 if (self->specialnum == 1){
				 self->target = get_nearest_target(self, other);
				 create_projectile_e(self, self->target,pool->twister, 5.0, 20, 4, 2, vector3d(0, 0, 0));
			 }
			 if (self->specialnum == 2){
				 self->target = get_nearest_target(self, other);
				 create_projectile_e(self, self->target, pool->meteor, 5.0, 20, 3, 2, vector3d(0, 0, -.9));
			 }
			 //create_hitbox(self, other, self->Hitbox.m_vecMin, self->Hitbox.m_vecMin, self->up);
			 //specialnum = 0;
		 }
	}
}
void mage_displacement(Entity *self, Vector3D disp){
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
			upx_m = disp.x;
			upy_m = disp.y;
			upz_m = disp.z;
		}
		gfc_matrix_translate(self->EntMatx, disp);
		gfc_matrix_copy(mtxcopy_m, self->EntMatx);
		//slog("up after x:%f y:%f z:%f frame:%f", self->up.x, self->up.y, self->up.z, framechange);
	}
	//
}
void update_mage_ent(Entity *self){
	self->EntMatx[3][2] = return_terrain_height(&other[6], -self->position.x, self->position.y);
	set_position(self, self->EntMatx);
	gfc_matrix_copy(mtxcopy_m, self->EntMatx);
	if (self->controling == 0){
		mage_ai_think(self);
	}
	self->frame = self->frame + 0.9;
	if(self->frame>=20&&self->state==ES_Attacking){
		//self->can_attack = true;
		self->can_block = true;
		//slog("can attack again attacknum_m:%i",attacknum_m);

}
	if (self->state == ES_Attacking){
		if (attacknum_m == 0&&self->frame==9){
			create_mage_projectile(self, 3.0, 1.0);
		}
		else if (attacknum_m == 3 && round(self->frame) == 35){
			//create_mage_projectile(self, 3.0, 1.0);
			create_projectile_e(self, NULL, pool->rock, 5.0, 20, false, 2, vector3d(0, 0, 0));
		}
		else if (attacknum_m == 5 && round(self->frame) == 39){
			//create_mage_projectile(self, 3.0, 1.0);
			create_projectile_e(self, NULL, pool->blastair, 5.0, 20, false, 2, vector3d(0, 0, 0));
		}
		

	}
	if (self->frame >= self->model->frameCount){
		if (self->state == ES_Attacking){
			self->frame = 0;
			self->state = ES_Idle;
			attacknum_m = 0;
			self->can_attack = true;
			self->can_block = true;
			self->update_model(self);
		}
		else if (self->state == ES_Hit){
			self->is_hit = false;
			self->in_action = false;
			self->state = ES_Idle;
			attacknum_m = 0;
			self->frame = 0;
			self->update_model(self);
		}
		else if (self->state == ES_Special){
			if (self->cast == 1){
				self->is_hit = false;
				self->can_special = true;
				self->can_block = true;
				self->can_attack = true;
				self->in_action = false;
				slog("end sp");
				self->specialnum = 0;
				self->frame = 0;
				self->cast = 0;
				self->action = none;
				self->state = ES_Idle;
				self->update_model(self);
			}
			else{
				self->cast = 1;
				self->state = ES_Special;
				self->update_model(self);
			}
		}
		else{
			self->in_action = false;
			up_trigger_m = false;
			down_trigger_m = false;
			self->frame = 0;
			self->update_model(self);
		}
		//slog("frame hit 0. state num:%i",self->state);
	}
	//slog("Frame %f", self->frame);
	
}

void mage_block(Entity *self){
	if (self->can_block == true){
		self->state = ES_Blocking;
		attacknum_m = 0;
		//slog("state: blocking");
		self->update_model(self);
	}


}
void init_mage_ent(Entity *self, int ctr, Entity *ents, glob_model_pool *pools){
	self->state = ES_Idle;
	self->dr = Up;
	self->attack = mage_attack;
	self->block = mage_block;
	self->update_model = update_mage_model;
	self->update_ent = update_mage_ent;
	self->think = mage_think;
	self->can_attack = true;
	self->can_hpskill = true;
	self->can_special = true;
	self->can_block = true;
	self->is_hit = false;
	self->action = none;
	self->prev_action = none;
	self->get_inputs = mage_get_inputs;
	self->up = vector3d(0, 1, 0);
	self->controling = ctr;
	self->movementspeed = 1.0f;
	self->rotated = 0.0f;
	self->type = ES_Player;
	other = ents;
	pool = pools;
	self->mods.idle = gf3d_model_load_animated("//player//mage//idle//mage_idle", "mage_s1", 0, 69);

	self->mods.run = gf3d_model_load_animated("//player//mage//run//mage_run", "mage_s1", 0, 16);

	self->mods.block = gf3d_model_load_animated("//player//mage//block//mage_block", "mage_s1", 0, 55);

	self->mods.hit = gf3d_model_load_animated("//player//mage//hit//mage_hit", "mage_s1", 0, 31);

	self->mods.attack1 = gf3d_model_load_animated("//player//mage//attack1//mage_attack1", "mage_s1", 0, 38);

	self->mods.attack2 = gf3d_model_load_animated("//player//mage//summon//summon", "mage_s1", 0, 68);

	self->mods.attack3 = gf3d_model_load_animated("//player//mage//blast//mage_blast", "mage_s1", 0, 87);

	self->mods.attack4 = gf3d_model_load_animated("//player//sword//sword_attack2//sword_s1_a2", "sword_s1", 0, 29);

	self->mods.special1 = gf3d_model_load_animated("//player//mage//charge//charge", "mage_s1", 0, 42);

	self->mods.special2 = gf3d_model_load_animated("//player//mage//cast//cast", "mage_s1", 26, 57);
	self->update_model(self);
	self->model->mesh[0]->minv.x += 2;
	gf3d_set_boundbox(self, self->model->mesh[0]->minv, self->model->mesh[0]->maxv);

	//other = ents;
}



void mage_get_inputs(Entity *self, const Uint8 *keys, float delta){
	framechange = delta;

	EntityState prev_state = self->state;
	self->prev_action = self->action;
	if (self->controling == 1){
		self->in_action = false;
		if (self->state != ES_Attacking&&self->state != ES_Blocking&&self->state != ES_Hit&&self->state!=ES_Special){
			if (keys[SDL_SCANCODE_RIGHT]){
				self->state = ES_Running;
				//update_model(self);
				self->dr = Right;
				dx_m = -1;
				//mage_displacement(self, vector3d(-self->movementspeed * framechange, 0,0));
				self->action = movement;
			}
			if (keys[SDL_SCANCODE_LEFT]){
				self->state = ES_Running;
				//update_model(self);
				self->dr = Left;
				dx_m = 1;
				self->action = movement;
			}
			if (keys[SDL_SCANCODE_UP]){
				self->state = ES_Running;
				//update_model(self);
				self->dr = Up;
				dy_m = 1;
				self->action = movement;
			}
			if (keys[SDL_SCANCODE_DOWN]){
				self->state = ES_Running;
				self->dr = Down;
				dy_m = -1;
				self->action = movement;
			}
			//slog("movemnt x:%f y:%f z:%f frame:%f",dx_m ,dy_m ,dz_m, framechange );
			mage_displacement(self, vector3d((dx_m*(self->movementspeed)), (dy_m*(self->movementspeed)), dz_m));

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
		switch (event.type){
			//player event
		case SDL_KEYUP:
			//self->action = none;
			dx_m = 0;
			dy_m = 0;
			dz_m = 0;
			if (self->state == ES_Running){
				self->state = ES_Idle;
				self->action = none;
				prev_action_m = none;
				//update_model(self);
				self->update_model(self);
			}
			switch (event.key.keysym.sym)
			{
			case SDLK_d: self->action = none; break;
			case SDLK_f: self->action = none; self->state = ES_Idle; self->update_model(self); break;
			case SDLK_UP: up_trigger_m = false; break;
			case SDLK_DOWN: down_trigger_m = false; break;
			default:
				break;
			}
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_c: self->attack(self); break;
			case SDLK_r: self->controling = 0; other[0].controling = 1; break;
			case SDLK_e: {
							 Vector3D d, e;
							 self->target = get_nearest_target(self, other);
							 vector3d_sub(d, self->position, self->target->position);
							 d.x = -d.x;
							 d.y = d.y;
							 d.z = 0;
							 e.x = upx_m;
							 e.y = upy_m;
							 e.z = upz_m;
							 vector3d_normalize(&d);
							 if (self->state != ES_Running){
								 rotate_towards_target(self, d, &e);
								 upx_m = e.x;
								 upy_m = e.y;
								 upz_m = e.z;
							 }}
				break;
			case SDLK_g: create_mage_projectile(&self, 3.0, 1.0); break;
			case SDLK_UP: up_trigger_m = true; break;
			case SDLK_DOWN: down_trigger_m = true; break;
			default:
				break;
			}
		default:
			break;

		}
	}
}

void create_mage_projectile(Entity *self, float speed, float dmg){
	Entity projEnt = *gf3d_entity_new();
	Model *proj = pool->blueorb;
	projEnt.model = proj;
	projEnt.up.x = upx_m; //self->up.x;
	projEnt.up.y = upy_m;//self->up.y;
	projEnt.up.z = upz_m;//self->up.z;
	//slog("up before x:%f y:%f z:%f ", self->up.x, self->up.y, self->up.z);
	//slog("up before x:%f y:%f z:%f ", projEnt.up.x, projEnt.up.y, projEnt.up.z);
	projEnt.movementspeed = speed;
	projEnt.attackdmg = dmg;
	projEnt.rotated = self->rotated;
	projEnt.state = ES_Idle;
	projEnt.update_ent = update_ent;
	projEnt.controling = 0;
	//projEnt.get_inputs = get_proj_inputs;
	projEnt.type = ES_Projectile;
	projEnt.ProjectileData.Hitarray = (Entity*)gfc_allocate_array(sizeof(Entity), 10);
	projEnt.ProjectileData.destroyOncollision = true;
	gfc_matrix_identity(projEnt.EntMatx);
	gfc_matrix_copy(projEnt.EntMatx, mtxcopy_m);
	//gfc_matrix_copy(projEnt.EntMatx, self->EntMatx);
	//ents[3] = &projEnt;	
	gf3d_set_boundbox(&projEnt, projEnt.model->mesh[0]->minv, projEnt.model->mesh[0]->maxv);
	//
	spawn_Entity2(&projEnt);
}
void mage_ai_think(Entity *self){
	if (self->controling != 1){
		float attack_range = 25.00f;
		Entity *target = get_nearest_target(self, other);
		self->target = target;
		if (target){
			//slog("distance between targets :%f", round(vector3d_magnitude_between(target->position, self->position)));
			if (vector3d_magnitude_between(target->position, self->position) < attack_range&&self->state != ES_Special){
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
					Vector3D d, f;
					vector3d_sub(d, self->position, target->position);
					d.x = -d.x;
					d.y = d.y;
					d.z = 0;
					f.x = upx_m;
					f.y = upy_m;
					f.z = upz_m;
					vector3d_normalize(&d);
					if (self->state != ES_Running){
						rotate_towards_target(self, d, &f);
						upx_m = f.x;
						upy_m = f.y;
						upz_m = f.z;
					}
					mage_attack(self);
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
					e.x = upx_m;
					e.y = upy_m;
					e.z = upz_m;
					vector3d_normalize(&d);
					if (self->state = ES_Running){
						rotate_towards_target(self, d, &e);
						upx_m = e.x;
						upy_m = e.y;
						upz_m = e.z;
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
					mage_special(self);
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
				c.x = round(c.x);
				c.y = round(c.y);
				c.z = 0;
				vector3d_normalize(&c);
				if (self->state != ES_Attacking&&self->state != ES_Idle){
					mage_displacement(self, vector3d(c.x*.35, c.y*.35, 0));
				}
				break;
			case block:
				if (self->can_block == true){
					mage_block(self);
					self->in_action = true;
					//self->update_model(self);
				}
				break;
			case none:
				if (self->state != ES_Attacking&&self->state!=ES_Special){
					self->state = ES_Idle;
					self->in_action = true;
					Vector3D d, e;
					vector3d_sub(d, self->position, target->position);
					d.x = -d.x;
					d.y = d.y;
					d.z = 0;
					e.x = upx_m;
					e.y = upy_m;
					e.z = upz_m;
					vector3d_normalize(&d);
					if (self->state != ES_Running){
						rotate_towards_target(self, d, &e);
						upx_m = e.x;
						upy_m = e.y;
						upz_m = e.z;
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