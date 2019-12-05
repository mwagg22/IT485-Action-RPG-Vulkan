#include "g_text.h"
#include <stdlib.h>
#include <string.h>
#include<math.h>
#include "simple_logger.h"
#include "g_control.h"
#include "g_entity.h"
#include "g_floor.h"
#include "g_swordhero.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"
#include "gf3d_texture.h"
#include <SDL_image.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "gf3d_swapchain.h"
typedef struct
{
	text_box               *textBox_list;
	Uint32                  max_textBox;
}TextManager;

 TextManager gf3d_textbox = { 0 };

void gf3d_text_manager_close()
{
	int i;
	for (i = 0; i < gf3d_textbox.max_textBox; i++)
	{
		gf3d_textbox.textBox_list[i]._inuse=0;
	}
	memset(&gf3d_textbox, 0, sizeof(TextManager));
	slog("model manager closed");
}

void gf3d_textbox_manager_init(Uint32 max_textBox)
{
	if (max_textBox == 0)
	{
		slog("cannot intilizat model manager for 0 models");
		return;
	}

	gf3d_textbox.textBox_list = (text_box *)gfc_allocate_array(sizeof(text_box), max_textBox);
	gf3d_textbox.max_textBox = max_textBox;
	slog("textbox manager initiliazed");
	//atexit(gf3d_text_manager_close);
}

text_box * gf3d_textbox_new()
{
	int i;
	for (i = 0; i < gf3d_textbox.max_textBox; i++)
	{
		if (!gf3d_textbox.textBox_list[i]._inuse)
		{
			slog("textbox index:%i", i);
			memset(&gf3d_textbox.textBox_list[i], 0, sizeof(text_box));
			//gf3d_model_delete(&gf3d_textbox.textBox_list[i]);
			gf3d_textbox.textBox_list[i]._inuse = 1;
			return &gf3d_textbox.textBox_list[i];
		}
	}
	slog("unable to make a new textbox, out of space");
	return NULL;
}

void create_textbox(char * text, float x, float y, float z, int endFrame, bool follow, Entity *target){
	text_box *textB;
	SDL_Surface * surface = NULL;
	textB = gf3d_textbox_new();
	Model *model;
	model = gf3d_model_new();
	model->frameCount = 2;
	model->mesh = (Mesh**)gfc_allocate_array(sizeof(Mesh*), 2);
	for (int i = 0; i < 2; i++)
	{
		model->mesh[i] = gf3d_mesh_load("../models//other//ui//face//sword//textbox.obj");
	}
	TTF_Init();
	TTF_Font * font = TTF_OpenFont("../sans.ttf", 10);
	SDL_Color color = { 255, 255, 255 };
	slog("creating for text:%s", text);
	surface = TTF_RenderText_Blended(font, text, color);
	textB->text_surf = surface;
	if (surface == NULL)
	{
		slog("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	model->texture = gf3d_surf_to_text(surface, text);
	textB->model = model;
	textB->endFrame = endFrame;
	textB->frame = 0;
	textB->position = vector3d(x, y, z);
	textB->follow = follow;
	textB->target = target;
	gfc_matrix_identity(textB->Matrix);
	if (target == NULL){
		gfc_matrix_make_translation(textB->Matrix, textB->position);
	}
	else{
		textB->Matrix[3][1] = textB->target->EntMatx[3][1] + textB->position.x;
		textB->Matrix[3][0] = textB->target->EntMatx[3][0] + textB->position.y;
		textB->Matrix[3][2] = textB->target->EntMatx[3][2] + textB->position.z;
	}
	gfc_matrix_rotate(textB->Matrix, textB->Matrix, 1.5708, vector3d(0, 0, 1));
	gfc_scale_matrix(textB->Matrix, strlen(text)+1 / 4, 1, 1);
	TTF_CloseFont(font);
	TTF_Quit();
}

void draw_text_boxes(Uint32 bufferFrame, VkCommandBuffer commandBuffer){
	text_box *b;
	for (int p = 0; p < gf3d_textbox.max_textBox; p++){
		if (gf3d_textbox.textBox_list[p]._inuse){
			b = &gf3d_textbox.textBox_list[p];
			gf3d_model_draw(b->model, bufferFrame, commandBuffer, b->Matrix, (Uint32)0);
			update_text_boxes(b);
		}
	}
}

void update_text_boxes(text_box *box){
	box->frame++;
	if (box->endFrame != NULL){
		if (box->frame == box->endFrame)
			;//box->_inuse = 0;
	}
	if (box->follow == true){
		box->Matrix[3][1] = box->target->EntMatx[3][1] + box->position.x;
		box->Matrix[3][0] = box->target->EntMatx[3][0] + box->position.y;
		box->Matrix[3][2] = box->target->EntMatx[3][2] + box->position.z;
	}
}
//itoa(int to string)
void update_textbox_texture(text_box *box,char* string){
	Texture *tex = box->model->texture;
	TTF_Init();
	SDL_Surface *surface= NULL;
	TTF_Font * font = TTF_OpenFont("../sans.ttf", 10);
	SDL_Color color = { 255, 255, 255 };
	slog("creating for text:%s", string);
	surface = TTF_RenderText_Blended(font, string, color);
	box->text_surf = surface;
	if (surface == NULL)
	{
		slog("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	box->model->texture = gf3d_surf_to_text(surface, string);
	gf3d_texture_delete(tex);
}