#include "gf3d_model.h"
#include "gf3d_commands.h"
//#include "gf3d_vgraphics.h"
#include "gf3d_obj_load.h"
#include "g_entity.h"


void goblin_think(Entity *self);
void goblin_attack(Entity *self);
void goblin_block(Entity *self);
void update_goblin_model(Entity *self);
void update_goblin_ent(Entity *self);
void init_goblin_ent(Entity *self, int ctr, Entity *ents, glob_model_pool *pools);
void goblin_set_position(Entity *self, Matrix4 mat);
void goblin_displacement(Entity *self, Vector3D disp);
void create_goblin_projectile(Entity *self, float speed, float dmg);