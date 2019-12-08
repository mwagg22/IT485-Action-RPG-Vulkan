#include "gf3d_model.h"
#include "gf3d_commands.h"
//#include "gf3d_vgraphics.h"
#include "gf3d_obj_load.h"
#include "g_entity.h"
//#include "g_text.h"
typedef struct health_s{
	struct text_s* box;
	struct text_s* mp_box;
	Model* model;
	Matrix4 EntMatrix;
	Vector3D position;
	Entity* target;
	Uint8 _inuse;
}healthbar;

typedef struct cursor_s{
	Model* model;
	Matrix4 EntMatrix;
	Vector3D position;
	int index;
	void(*update_cursor) (struct cursor_s *self);
	bool show;
}cursor;

typedef struct menu_item_s{
	Model* model;
	Matrix4 EntMatrix;
	Vector3D position;
	int index;
	bool show;
}Menu_item;

void gf3d_hud_manager_close();
void gf3d_hud_manager_init(Uint32 max_hudBox);
healthbar * gf3d_hud_new();
void update_hud_model(Entity *self);
void update_hud_ent(Entity *self);
void init_hud_ent(Entity *self, int ctr, Entity *ents);
//void hud_set_position(Matrix4 self, Matrix4 mat, int x, int y, int z);
void hud_displacement(Entity *self, Vector3D disp);
void draw_huds(Uint32 bufferFrame, VkCommandBuffer commandBuffer);
healthbar* create_healthbar(char *model, char *texture, float x, float y, float z, Entity *target);
void hud_set_position(healthbar *self);
//cursor stuff
void init_cursor(cursor *self);
void cursor_inputs(cursor *self, const Uint8 * keys);
void update_newgame(cursor *self);
void update_continue(cursor *self);
void update_quit(cursor *self);
void update_cursor(cursor *self, Menu_item* menuitems);
void init_menu_item(Menu_item *self, int num);