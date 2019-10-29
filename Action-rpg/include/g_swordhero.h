#include "gf3d_model.h"
#include "gf3d_commands.h"
//#include "gf3d_vgraphics.h"
#include "gf3d_obj_load.h"
#include "g_entity.h"


void sword_think(Entity *self);
void sword_attack(Entity *self);
void sword_ai_think(Entity *self);
void sword_special(Entity *self);
void sword_block(Entity *self);
void update_sword_model(Entity *self);
void update_sword_ent(Entity *self);
void init_sword_ent(Entity *self,int ctr,Entity *ents);
void sword_set_position(Entity *self, Matrix4 mat);
//void sword_displacement(Entity *self, Vector3D disp);
void sword_get_inputs(Entity *self, const Uint8 * keys, float delta);