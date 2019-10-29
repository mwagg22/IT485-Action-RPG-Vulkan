#include "gf3d_model.h"
#include "gf3d_commands.h"
//#include "gf3d_vgraphics.h"
#include "gf3d_obj_load.h"
#include "g_entity.h"


void mage_think(Entity *self);
void mage_ai_think(Entity *self);
void mage_attack(Entity *self);
void mage_special(Entity *self);
void mage_block(Entity *self);
void update_mage_model(Entity *self);
void update_mage_ent(Entity *self);
void init_mage_ent(Entity *self,int ctr,Entity *ents);
void mage_set_position(Entity *self, Matrix4 mat);
//void mage_displacement(Entity *self, Vector3D disp);
void mage_get_inputs(Entity *self, const Uint8 * keys, float delta);
void create_mage_projectile(Entity *self, float speed, float dmg);