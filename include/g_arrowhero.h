#include "gf3d_model.h"
#include "gf3d_commands.h"
//#include "gf3d_vgraphics.h"
#include "gf3d_obj_load.h"
#include "g_entity.h"


void arrow_think(Entity *self);
void arrow_ai_think(Entity *self);
void arrow_attack(Entity *self);
void arrow_special(Entity *self);
void arrow_block(Entity *self);
void update_arrow_model(Entity *self);
void update_arrow_ent(Entity *self);
void init_arrow_ent(Entity *self, int ctr, Entity *ents, glob_model_pool *pools);
void arrow_set_position(Entity *self, Matrix4 mat);
//void arrow_displacement(Entity *self, Vector3D disp);
void arrow_get_inputs(Entity *self, const Uint8 * keys, float delta);
void create_arrow_projectile(Entity *self, float speed, float dmg);