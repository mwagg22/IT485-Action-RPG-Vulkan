#include <SDL.h> 
#include <SDL_ttf.h>
#include <stdbool.h>
#include<math.h>
#include<time.h> 
#include "gf3d_obj_load.h"
#include "g_entity.h"
#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"
#include "g_entity.h"
#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gf3d_texture.h"
#include "g_timer.h"
#include "g_swordhero.h"
#include "g_magehero.h"
#include "g_arrowhero.h"
#include "g_floor.h"
#include "g_goblin.h"
#include "g_control.h"
#include "g_hud.h"

#include "g_text.h"
Entity *ents2;
Menu_item *menuitems;
//Entity *ents2;
glob_model_pool *pools;
Uint32 entity_max=100;
int created = 6;

void spawn_Entity2(Entity *entsToAdd){
	for (int i = 0; i < entity_max; i++){
		if (ents2[i]._inuse!=1){
			memset(&ents2[i], 0, sizeof(Entity));
			ents2[i] = *entsToAdd;
			created++;
			slog("Added at :%i and created :%i",i,created);
			return;
		}
	}
}

void hide_all(Entity *list){
	for (int i = 0; i < entity_max; i++){
		if (ents2[i]._inuse){
			hide(&ents2[i]);
		}
	}
}

void show_all(Entity *list){
	for (int i = 0; i < entity_max; i++){
		if (ents2[i]._inuse){
			show(&ents2[i]);
		}
	}
}

Entity *return_game_list(){
	return ents2;
}
glob_model_pool *return_model_pool(){
	return pools;
}

int main(int argc,char *argv[])
{
	Matrix4 gf3d_camera = {0};
	ents2 = (Entity*)gfc_allocate_array(sizeof(Entity), entity_max);	
	pools = (glob_model_pool*)gfc_allocate_array(sizeof(glob_model_pool), 12);
	menuitems = (Menu_item*)gfc_allocate_array(sizeof(Menu_item), 4);
    int done = 0;
    int a;
	SDL_Event event;
	SDL_Renderer *renderer;
	float frame = 0;
    Uint8 validate = 0;
	Uint32 startTime=0;
	Uint32 bufferFrame = 0;
    const Uint8 * keys;
	VkCommandBuffer commandBuffer, commandBuffer2;
	gf3d_textbox_manager_init((Uint32)1024);
	Entity Ent;
	Entity Ent2;
	Entity Ent3;
	Entity Ent4;
	Entity Ent5;
	Entity Ent6;
	Entity Ent7;
	Entity Ent8;
	Entity Ent9;//hud
	Entity Ent10;
	Entity Ent11;
	Entity Ent12;
	Menu_item Ment1, Ment2, Ment3;
	cursor curs;
	LTimer timer;
	long lastFrameTime;
	float deltaTime;
	LTimer_init(&timer);
	start(&timer);
	int countedframes=0;
	bool inMenu = true;
	Entity background;
	
    for (a = 1; a < argc;a++)
    {
        if (strcmp(argv[a],"-disable_validate") == 0)
        {
            validate = 0;
        }
    }
	//startTime = SDL_GetTicks();
    init_logger("gf3d.log");    
    slog("gf3d begin");
    gf3d_vgraphics_init(
        "gf3d",                 //program name
        1200,                   //screen width
        700,                    //screen height
        vector4d(0.51,0.75,1,1),//background color
        0,                      //fullscreen
        validate                //validation
    );
	
    // main game loop
    slog("gf3d main loop begin");
	init_cursor(&curs);
	init_menu_item(&Ment1, 0);
	init_menu_item(&Ment2, 1);
	init_menu_item(&Ment3, 2);
	menuitems[0] = Ment1;
	menuitems[1] = Ment2;
	menuitems[2] = Ment3;
	init_global_model_pool(pools);
	Ent = *gf3d_entity_new();
	Ent2 = *gf3d_entity_new();
	Ent3 = *gf3d_entity_new();
	Ent4 = *gf3d_entity_new();
	Ent5 = *gf3d_entity_new();
	Ent6 = *gf3d_entity_new();
	Ent7 = *gf3d_entity_new();
	Ent8 = *gf3d_entity_new();
	Ent9 = *gf3d_entity_new();
	Ent10 = *gf3d_entity_new();
	Ent11 = *gf3d_entity_new();
	Ent12 = *gf3d_entity_new();
	gfc_matrix_identity(Ent.EntMatx);  	
	gfc_matrix_identity(Ent2.EntMatx);
	gfc_matrix_identity(Ent3.EntMatx);
	gfc_matrix_identity(Ent4.EntMatx);
	gfc_matrix_identity(Ent5.EntMatx);
	gfc_matrix_identity(Ent6.EntMatx);
	gfc_matrix_identity(Ent7.EntMatx);
	gfc_matrix_identity(Ent8.EntMatx);
	gfc_matrix_identity(Ent9.EntMatx);
	gfc_matrix_identity(Ent10.EntMatx);
	gfc_matrix_identity(Ent11.EntMatx);
	gfc_matrix_identity(Ent12.EntMatx);
	gfc_matrix_make_translation(
		Ent.EntMatx,
		vector3d(-400, -400, 100)
		);
	gfc_matrix_make_translation(
		Ent4.EntMatx,
		vector3d(-400, -300, 0)
		);
	gfc_matrix_make_translation(
		Ent5.EntMatx,
		vector3d(0, 0, 0)
		);
	gfc_matrix_make_translation(
		Ent6.EntMatx,
		vector3d(-450, -300, 0)
		);
	gfc_matrix_make_translation(
		Ent7.EntMatx,
		vector3d(0, 0, -1)
		);
	gfc_matrix_make_translation(
		Ent8.EntMatx,
		vector3d(0, 0, 100)
		);
	gfc_matrix_make_translation(
		Ent9.EntMatx,
		vector3d(0, 0, 0)
		);
	gfc_matrix_make_translation(
		Ent10.EntMatx,
		vector3d(0, 0, 100)
		);
	gfc_matrix_make_translation(
		Ent11.EntMatx,
		vector3d(0, 0, 100)
		);
	gfc_matrix_make_translation(
		Ent12.EntMatx,
		vector3d(0, 800, 100)
		);
	init_sword_ent(&Ent, 1, ents2,pools);
	init_arrow_ent(&Ent2, 0, ents2,pools);
	init_mage_ent(&Ent3, 0, ents2,pools);
	init_goblin_ent(&Ent4, 1, ents2,pools);
	//init_floor_ent(&Ent5, 1, ents2,pools);
	init_goblin_ent(&Ent6, 1, ents2,pools);
	init_floor_ent(&Ent7, 0, ents2);
	init_wall_ent(&Ent8, 0, ents2);
	init_floor_ent(&Ent9, 5, ents2);
	init_floor_ent(&Ent10, 3, ents2);
	init_floor_ent(&Ent11, 4, ents2);
	//init_floor_ent(&Ent12, 6, ents2);
	ents2[0] = Ent;
	ents2[1] = Ent2;
	ents2[2] = Ent3;
	ents2[3] = Ent4;
	//ents2[4] = Ent5;
	ents2[5] = Ent6;
	ents2[6] = Ent7;
	ents2[12] = Ent8;
	ents2[8] = Ent9;
	ents2[10] = Ent10;
	ents2[11] = Ent11;
	//ents2[12] = Ent12;
	float rot=0;
	float wheel = 0;
	srand(time(0));
	float posy = -20;
	for (int p = 0; p < 10; p++){
		Entity N = *gf3d_entity_new();
		gfc_matrix_identity(N.EntMatx);
		gfc_matrix_make_translation(
			N.EntMatx,
			vector3d(-250, posy, 0)
			);
		posy -= 20;
		init_floor_ent(&N, 2, ents2);
		spawn_Entity2(&N);
		//slog("position x:%f y:%f z:%f", N.position.x, N.position.y, N.position.z);
	}
	hide_all(ents2);
	gfc_matrix_identity(background.EntMatx);
	background.model = gf3d_model_load_animated("//other//ui//face//sword//sword_face", "//other//ui//menu//titlescreen", 0, 2);
	gfc_scale_matrix(background.EntMatx, 40, 20, 20);
	gfc_matrix_rotate(background.EntMatx, background.EntMatx, -1.5708, vector3d(0, 0, 1));
	background.EntMatx[3][2] = -20;
	background.EntMatx[3][1] = -2;
	background.frame = 0;
	//quadt *quad_terrain=return_qtree(quad_terrain);
	//gf3d_set_camera(gf3d_camera, &ents2[0], rot);
	//Node *n = malloc(sizeof (Node));
    while(!done)
	{
		inMenu = curs.show;
		lastFrameTime = SDL_GetTicks();
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //update game things here
		if (keys[SDL_SCANCODE_SPACE]){
			//gfc_matrix_view(,Ent.position,vector3d(0,0,1))
		  rot+=0.05;
		  //Ent.rotated =-rot;
		}
		//slog("Time in millisecs: %f", (SDL_GetTicks() - startTime)/(1000.f));
		/*float avgFPS = countedframes / (getTicks(&timer) / 1000.f);
		slog("average frames:%f", avgFPS);
		if (avgFPS > 2000000)
		{
			avgFPS = 0;
		}*/
        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        bufferFrame = gf3d_vgraphics_render_begin();
        gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_pipeline(),bufferFrame);
	    gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_pipeline2d(), bufferFrame);
            commandBuffer = gf3d_command_rendering_begin(bufferFrame);
			commandBuffer2 = gf3d_command_rendering_begin_2d(bufferFrame);
			countedframes++;
			if (inMenu == false){
				for (int p = 0; p < entity_max; p++){
					if (ents2[p]._inuse&&ents2[p].show == true){
						//slog("index :%i", p);
						gf3d_model_draw(ents2[p].model, bufferFrame, commandBuffer, ents2[p].EntMatx, (Uint32)ents2[p].frame);
						if (ents2[p].controling == 1){
							ents2[p].get_inputs(&ents2[p], keys, deltaTime);
							gf3d_set_camera(gf3d_camera, &ents2[p], rot, wheel);
						}
						ents2[p].update_ent(&ents2[p]);
					}
				}
				//update_skybox(&ents2[4], ents2[0].position);
				draw_huds(bufferFrame, commandBuffer2);
				draw_text_boxes(bufferFrame, commandBuffer2);
				collision_check(ents2, entity_max);
				update_battle_manager(0, 0);
			}
			else{
				gf3d_model_draw(background.model, bufferFrame, commandBuffer, background.EntMatx, (Uint32)1);
				//gf3d_set_camera(gf3d_camera, &background, rot, wheel);
				gf3d_model_draw(curs.model, bufferFrame, commandBuffer, curs.EntMatrix, (Uint32)0);
				update_cursor(&curs, menuitems);
				cursor_inputs(&curs, keys);
				for (int i = 0; i < 3; i++){
					gf3d_model_draw(menuitems[i].model, bufferFrame, commandBuffer, menuitems[i].EntMatrix, (Uint32)0);
				}
				
			}
            gf3d_command_rendering_end(commandBuffer);
			gf3d_command_rendering_end(commandBuffer2);
        gf3d_vgraphics_render_end(bufferFrame);
		//SDL_Delay((1000 / 60) - SDL_GetTicks());
		deltaTime = (SDL_GetTicks() - lastFrameTime)/1000.0f;
		//slog("last fps :%f change:%f", lastFrameTime/1000.f,deltaTime);
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
    }    
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    //cleanup
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
