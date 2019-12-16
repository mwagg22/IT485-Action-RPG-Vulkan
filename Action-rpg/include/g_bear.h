#include "gf3d_model.h"
#include "gf3d_commands.h"
//#include "gf3d_vgraphics.h"
#include "gf3d_obj_load.h"
#include "g_entity.h"


void bear_think(Entity *self);
void bear_attack(Entity *self);
void bear_special(Entity *self);
void bear_block(Entity *self);
void update_bear_model(Entity *self);
void update_bear_ent(Entity *self);
void init_bear_ent(Entity *self, int ctr, Entity *ents, glob_model_pool *pools);
void bear_set_position(Entity *self, Matrix4 mat);
void bear_displacement(Entity *self, Vector3D disp);
void create_bear_projectile(Entity *self, float speed, float dmg);