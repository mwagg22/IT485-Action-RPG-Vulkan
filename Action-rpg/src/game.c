#include <SDL.h>            
#include <stdbool.h>
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

Entity *ents2;
Uint32 entity_max=20;
int created = 6;
VkCommandBuffer commandBuffer;
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

int main(int argc,char *argv[])
{
	Matrix4 gf3d_camera = {0};
	ents2 = (Entity*)gfc_allocate_array(sizeof(Entity), entity_max);	
    int done = 0;
    int a;
	SDL_Event event;
	float frame = 0;
    Uint8 validate = 0;
	Uint32 startTime=0;
	Uint32 bufferFrame = 0;
    const Uint8 * keys;
	Entity Ent;
	Entity Ent2;
	Entity Ent3;
	Entity Ent4;
	Entity Ent5;
	Entity Ent6;
	Entity Ent7;
	LTimer time;
	long lastFrameTime;
	float deltaTime;
	LTimer_init(&time);
	start(&time);
	int countedframes=0;
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
	Ent = *gf3d_entity_new();
	Ent2 = *gf3d_entity_new();
	Ent3 = *gf3d_entity_new();
	Ent4 = *gf3d_entity_new();
	Ent5 = *gf3d_entity_new();
	Ent6 = *gf3d_entity_new();
	Ent7 = *gf3d_entity_new();
	gfc_matrix_identity(Ent.EntMatx);
	gfc_matrix_identity(Ent3.EntMatx);    	
	gfc_matrix_identity(Ent2.EntMatx);
	gfc_matrix_identity(Ent4.EntMatx);
	gfc_matrix_identity(Ent5.EntMatx);
	gfc_matrix_identity(Ent6.EntMatx);
	gfc_matrix_identity(Ent7.EntMatx);
	gfc_matrix_make_translation(
		Ent.EntMatx,
		vector3d(-16, -16, 0)
		);
    gfc_matrix_make_translation(
		Ent2.EntMatx,
            vector3d(25,0,0)
        );
	gfc_matrix_make_translation(
		Ent3.EntMatx,
		vector3d(-16, 0, 0)
		);
	gfc_matrix_make_translation(
		Ent4.EntMatx,
		vector3d(16, 16, 0)
		);
	gfc_matrix_make_translation(
		Ent5.EntMatx,
		vector3d(10, 20, 2)
		);
	gfc_matrix_make_translation(
		Ent6.EntMatx,
		vector3d(15, 20, 0)
		);
	gfc_matrix_make_translation(
		Ent7.EntMatx,
		vector3d(0, 0, 0)
		);
	init_sword_ent(&Ent, 1, ents2);
	init_arrow_ent(&Ent2, 0, ents2);
	init_mage_ent(&Ent3, 0, ents2);
	init_goblin_ent(&Ent4, 0, ents2);
	init_bee_ent(&Ent5, 0, ents2);
	init_goblin_ent(&Ent6, 0, ents2);
	init_floor_ent(&Ent7, 0, ents2);
	Ent4.Ent_ID = 3;
	Ent5.Ent_ID = 4;
	Ent6.Ent_ID = 5;
	Ent7.Ent_ID = 6;
	ents2[0] = Ent;
	ents2[1] = Ent2;
	ents2[2] = Ent3;
	ents2[3] = Ent4;
	ents2[4] = Ent5;
	ents2[5] = Ent6;
	ents2[6] = Ent7;

/*	ents_list[0] = &Ent2;
	ents_list[1] = &Ent3;
	ents_list[2] = &Ent;*/	
	float rot=0;
	//gf3d_set_camera(gf3d_camera, &ents2[0], rot);
    while(!done)
	{
		
		lastFrameTime = SDL_GetTicks();
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //update game things here
		if (keys[SDL_SCANCODE_SPACE]){
			//gfc_matrix_view(,Ent.position,vector3d(0,0,1))
		  rot+=0.9;
		  //Ent.rotated =-rot;
		}
		//slog("Time in millisecs: %f", (SDL_GetTicks() - startTime)/(1000.f));
		//float avgFPS = countedframes / (getTicks(&time) / 1000.f);
		//if (avgFPS > 2000000)
	//	{
	//		avgFPS = 0;
		//}
        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        bufferFrame = gf3d_vgraphics_render_begin();
        gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_pipeline(),bufferFrame);
            commandBuffer = gf3d_command_rendering_begin(bufferFrame);
			countedframes++;
			for (int p = 0; p < entity_max; p++){
				if (ents2[p]._inuse&&ents2[p].model->_inuse){
					//slog("index :%i", p);
					gf3d_model_draw(ents2[p].model, bufferFrame, commandBuffer, ents2[p].EntMatx, (Uint32)ents2[p].frame);
					if (ents2[p].controling == 1){
						ents2[p].get_inputs(&ents2[p], keys, deltaTime);
						gf3d_set_camera(gf3d_camera, &ents2[p], rot);
					}
					ents2[p].update_ent(&ents2[p]);
				}
			}
			collision_check(ents2, entity_max);
            gf3d_command_rendering_end(commandBuffer);
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
