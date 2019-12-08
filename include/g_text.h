#include "gf3d_model.h"
#include "g_includes.h"
//#include "gf3d_vgraphics.h"
#include "gf3d_obj_load.h"
#include "g_entity.h"


typedef struct text_s{
	SDL_Surface *text_surf;
	Model *model;
	Vector3D position;
	Matrix4 Matrix;
	Uint8 _inuse;         /**<flag to keep track if this isntance is in use and should not be reassigned*/
	Entity *target;		  //entity the camera focuses on to update position
	int endFrame;
	int frame;
	bool follow;
}text_box;

void gf3d_text_manager_close();
void gf3d_textbox_manager_init(Uint32 max_textBox);
text_box * gf3d_textbox_new();
text_box* create_textbox(char * text, float x, float y,float z,int endFrame,bool follow,Entity *target);
//Texture *gf3d_surf_to_text(SDL_Surface *surface);
void draw_text_boxes(Uint32 bufferFrame, VkCommandBuffer commandBuffer);
void update_text_boxes(text_box* box);
void update_textbox_texture(text_box *box, char* string);