#include "gf3d_model.h"
#include "gf3d_commands.h"
//#include "gf3d_vgraphics.h"
#include "gf3d_obj_load.h"
#include "g_entity.h"


void bee_think(Entity *self);
void bee_attack(Entity *self);
void bee_block(Entity *self);
void update_bee_model(Entity *self);
void update_bee_ent(Entity *self);
void init_bee_ent(Entity *self,int ctr,Entity *ents);
void bee_set_position(Entity *self, Matrix4 mat);
void bee_displacement(Entity *self, Vector3D disp);
void create_bee_projectile(Entity *self, float speed, float dmg);