#include <stdio.h>
#include<math.h>
#include "gf3d_obj_load.h"
#include "simple_logger.h"
#include "g_floor.h"
#include "g_entity.h"
Entity *other;
Vector3D heights[80][80];
quadt *quad_terrain;
void create_quad(quadt* quad,Vector2D UL, Vector2D BR){
	//slog("creating");
	slog("new quad created");
	quad->n = NULL;
	quad->PuL = UL;
	quad->PbR = BR;
	quad->UL = NULL;
	quad->BL = NULL;
	quad->UR = NULL;
	quad->BR = NULL;
	slog("here");
}
void init_floor_ent(Entity *self, int ctr, Entity *ents){
	self->state = ES_Idle;
	self->dr = Up;
	//self->attack = mage_attack;
	//self->block = mage_block;
	//self->update_model = update_mage_model;
	self->update_ent = update_ent;
	//self->think = mage_think;
	self->can_attack = true;
	self->can_hpskill = true;
	self->can_special = true;
	self->can_block = true;
	self->is_hit = false;
	self->action = none;
	self->prev_action = none;
	//self->get_inputs = mage_get_inputs;
	self->up = vector3d(0, 1, 0);
	self->controling = 0;
	self->movementspeed = 1.0f;
	self->rotated = 0.0f;
	self->type = ES_Stage;
	other = ents;
	self->show = true;
	if (ctr == 0){
		self->model = gf3d_model_load_animated("//other//level//grass//grass//grass", "grass", 0, 1);

		load_terrain_data("../models//other//level//grass//grass//grass.obj");
	}
	if (ctr == 1){
		self->model = gf3d_model_load_animated("//other//level//skybox//skybox", "sky", 0, 1);
	}
	if (ctr == 2){
		self->model = gf3d_model_load_animated("//other//level//trees//trees", "tree", 0, 1);
		set_position(self, self->EntMatx);
		self->EntMatx[3][2] = return_terrain_height(&other[6], -self->position.x, self->position.y);
	}
	if (ctr == 3){
		self->model = gf3d_model_load_animated("//other//level//planes//grass", "//other//level//grass", 0, 1);
		self->show = false;
	}
	if (ctr == 4){
		self->model = gf3d_model_load_animated("//other//level//skybox//battle//1//grass", "//other//level//bgrass", 0, 1);
		self->show = false;
	}
	if (ctr == 5){
		self->model = gf3d_model_load_animated("//other//level//planes//water", "//other//level//water", 0, 1);
	}
	if (ctr == 6){
		self->model = gf3d_model_load_animated("//other//level//cave//cave", "//other//level//cave", 0, 2);
		self->show = false;
		//load_terrain_data("../models//other//level//grass//grass//grass.obj");
	}
	if (ctr == 7){
		self->model = gf3d_model_load_animated("//other//level//warps//cave", "//other//level//warpcave", 0, 1);
		self->type = ES_Warp;
		self->location = vector3d(-570, -385, 0);
		gfc_matrix_rotate(self->EntMatx, self->EntMatx, 3.14, vector3d(0,0,1));
		gf3d_set_boundbox(self, self->model->mesh[0]->minv, self->model->mesh[0]->maxv);
	}
	//quadt quad_terrain2;
	// quad_terrain = malloc(sizeof (quadt));
	//create_quad(quad_terrain, vector2d(0, 0), vector2d(-180, 180));
	//create_quad(&quad_terrain2, vector2d(0, 0), vector2d(-200, 400));
	//for (int i = 0; i < 8; i++){
//		for (int j = 0; j < 8; j++){
	//		insert_to_quad(quad_terrain, vector2d(0,0), vector2d(-180, 180), j, i);
	//	}
	//}
	//Node *n = malloc(sizeof (Node));
	//search_closest_quad(quad_terrain, &other[0], n);
}
void init_wall_ent(Entity *self, int ctr, Entity *ents){
	self->state = ES_Idle;
	self->dr = Up;
	self->update_ent = update_ent;
	self->can_attack = true;
	self->can_hpskill = true;
	self->can_special = true;
	self->can_block = true;
	self->is_hit = false;
	self->action = none;
	self->prev_action = none;
	self->up = vector3d(0, 1, 0);
	self->controling = 0;
	self->movementspeed = 1.0f;
	self->rotated = 0.0f;
	self->type = ES_Stage;
	other = ents;
	self->show = true;
	if (ctr == 0){
		self->model = gf3d_model_load_animated("//other//level//wall//grass//grass", "//other//level//g_wall", 0, 2);
	}
	if (ctr == 1){
		self->model = gf3d_model_load_animated("//other//level//skybox//skybox", "sky", 0, 2);
	}
}
float barryCentric(Vector3D p1, Vector3D p2, Vector3D p3, Vector2D pos) {
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}
float return_terrain_height(Entity* terr,float x,float y){
	float answer;
	float terrainx = x - terr->position.x;
	float terrainy = y + terr->position.y;
	float gridSquareSize = (800.0 / (79));
	int gridx = (int)floorf(terrainx / gridSquareSize);
	int gridy = (int)floorf(terrainy / gridSquareSize);
	if (gridx > 79 || gridy > 79 || gridx < 0 || gridy < 0){
		return 0;
	}
	float xcoord = fmodf(terrainx, gridSquareSize)/ gridSquareSize;
	
	float ycoord = fmodf(terrainy, gridSquareSize) / gridSquareSize;
	//slog("x:%i y:%i", gridx, gridy);
	//slog("xcorrd:%f ycorrd:%f", xcoord, ycoord);
	if (xcoord <= (1 - ycoord)) {
		answer = barryCentric(vector3d(0, heights[gridx][gridy].z, 0), vector3d(1,
			heights[gridx + 1][gridy].z, 0), vector3d(0,
			heights[gridx][gridy + 1].z, 1), vector2d(xcoord, ycoord));
	}
	else {
		answer = barryCentric(vector3d(1, heights[gridx + 1][gridy].z, 0), vector3d(1,
			heights[gridx + 1][gridy + 1].z, 1), vector3d(0,
			heights[gridx][gridy + 1].z, 1), vector2d(xcoord, ycoord));
	}
	if ((isnan(answer))){
		answer = 0;
	}
	if ((isinf(answer))){
		answer = 0;
	}
	//slog("Height:%f", answer);
	return answer;
}
void load_terrain_data(char *filename)
{
	
	int  numvertices = 0;
	int  numnormals = 0;
	int  numtexcoords = 0;
	int  numfaces = 0;
	char buf[128];
	float x,y,z;
	int k = 0;
	Vector3D verts[6500];
	int f[3][3];
	FILE* file = fopen(filename, "r");
	if (file == NULL)
		return;
	//slog("%s", file);

	rewind(file);
	while (fscanf(file, "%s", buf) != EOF)
	{
		switch (buf[0])
		{
		case 'v':
			switch (buf[1])
			{
			case '\0':
				fscanf(
					file,
					"%f %f %f",
					&x,
					&y,
					&z
					);
				
				verts[numvertices] = vector3d(x,y,z);
				//verts[numvertices+1] = y;
				//verts[numvertices+2] = z;
				//slog("vert: %f", verts[numvertices]);
				
				//want to store z so height[y][x]=z;
				numvertices++;
				break;
			case 'n':
				fscanf(
					file,
					"%f %f %f",
					&x,
					&y,
					&z
					);

				break;
			case 't':
				fscanf(
					file,
					"%f %f",
					&x,
					&y
					);

				break;
			default:
				break;
			}
			break;
		case 'f':
			fscanf(
				file,
				"%d/%d/%d %d/%d/%d %d/%d/%d",
				&f[0][0],
				&f[0][1],
				&f[0][2],

				&f[1][0],
				&f[1][1],
				&f[1][2],

				&f[2][0],
				&f[2][1],
				&f[2][2]);
			break;
			default:
			fgets(buf, sizeof(buf), file);
			break;
		}
	}
	
	for (int i = 0; i<80; i++){
		for (int j = 79; j>-1; j--){
			heights[j][i] = verts[k];
			//slog("vert:%f ind:%i", heights[j][i].z,k);
			k++;
		}
	}
	fclose(file);
}

void insert_to_quad(quadt* quad, Vector2D PuL, Vector2D PbR, int Jindx, int Iindx){
	Node *n = malloc(sizeof (Node));
	quadt *temp = malloc(sizeof (quadt));
	slog("inserting:%f , %f ,%f ", heights[Jindx][Iindx].x, heights[Jindx][Iindx].y, heights[Jindx][Iindx].z);
	// Current quad cannot contain it 
	if (!inBoundary(heights[Jindx][Iindx], PuL, PbR)){
		slog("Not in bounds:%f , %f", heights[Jindx][Iindx].x, heights[Jindx][Iindx].y);
		return;
	}

	// We are at a quad of unit area 
	// We cannot subdivide this quad further 
	if (abs(PuL.x - PbR.x) <= 20 &&
		abs(PuL.y - PbR.y) <= 20)
	{
		if (quad->n == NULL){
			n->pointUL = heights[Jindx][Iindx];
			n->pointBL = heights[Jindx + 1][Iindx];
			n->pointUR = heights[Jindx][Iindx + 1];
			n->pointBR = heights[Jindx + 1][Iindx + 1];
			n->i = Iindx;
			n->j = Jindx;
			quad->n = n;
			quad->isLeaf = true;
			slog("inserted");
		}
		return;
	}
	if ((PuL.x + PbR.x) / 2 <= heights[Jindx][Iindx].x)
	{
		// Indicates topLeftTree 
		if ((PuL.y + PbR.y) / 2 >= heights[Jindx][Iindx].y)
		{
			if (quad->UL == NULL){
				slog("top left");
				//create_quad(Vector2D UL, Vector2D BR)
				create_quad(temp, vector2d(PuL.x, PuL.y), vector2d((PuL.x + PbR.x) / 2, (PuL.y + PbR.y) / 2));
				quad->UL = temp;
			}
				//slog("new ul corrd: upper x:%f ,bottom y:%f", quad->UL->PuL.x, quad->UL->PbR.y);
			insert_to_quad(quad->UL, vector2d(PuL.x, PuL.y), vector2d((PuL.x +PbR.x) / 2, (PuL.y +PbR.y) / 2), Jindx, Iindx);
			//topLeftTree->insert(node);
		}

		// Indicates botLeftTree 
		else
		{
			if (quad->BL == NULL){
				slog("bottom left");
				create_quad(temp, vector2d(quad->PuL.x, (quad->PuL.y + quad->PbR.y) / 2), vector2d((quad->PuL.x + quad->PbR.x) / 2, quad->PbR.y));
				quad->BL = temp;
				
				//botLeftTree->insert(node);
			}
			insert_to_quad(quad->BL, vector2d(PuL.x, (PuL.y + PbR.y) / 2), vector2d((PuL.x +PbR.x) / 2,PbR.y), Jindx, Iindx);
		}
	}
	else
	{
		// Indicates topRightTree 
		if ((PuL.y + PbR.y) / 2 >= heights[Jindx][Iindx].y)
		{
			if (quad->UR == NULL){
				slog("top right");
				create_quad(temp, vector2d((quad->PuL.x + quad->PbR.x) / 2, quad->PuL.y), vector2d(quad->PbR.x, (quad->PuL.y + quad->PbR.y) / 2));
				quad->UR = temp;
				
				//topRightTree->insert(node);
			}
			insert_to_quad(quad->UR, vector2d((PuL.x + PbR.x) / 2, quad->PuL.y), vector2d(PbR.x, (PuL.y +PbR.y) / 2), Jindx, Iindx);
		}

		// Indicates botRightTree 
		else
		{
			if (quad->BR == NULL){
				slog("bottom right");
				create_quad(temp, vector2d((quad->PuL.x + quad->PbR.x) / 2, (quad->PuL.y + quad->PbR.y) / 2), vector2d(quad->PbR.x, quad->PbR.y));
				quad->BR = temp;
				
				//botRightTree->insert(node);
			}
			insert_to_quad(quad->BR, vector2d((PuL.x + PbR.x) / 2, (PuL.y + PbR.y) / 2), vector2d(PbR.x, PbR.y), Jindx, Iindx);
		}
	}
}
bool inBoundary(Vector3D p, Vector2D PuL, Vector2D PbR)
{
	slog("upperx:%f , lowerx:%f", PuL.x, PbR.x);
	slog("uppery:%f , lowery:%f", PuL.y, PbR.y);
	return (p.x <= PuL.x &&
		p.x >= PbR.x &&
		p.y >= PuL.y &&
		p.y <= PbR.y);
}
float search_closest_quad(quadt *quad, Entity *ents,Node* near){
	slog("First index:x:%f y:%f z:%f", heights[0][0].x, heights[0][0].y, heights[0][0].z);
	if (!inBoundary(ents->position, quad->PuL, quad->PbR)){
		slog("Not in bounds:");
		return;
	}
	if (quad->n != NULL){
		near = quad->n;
		slog("nearest x:%f", near->pointUL.y);
		ents->EntMatx[3][2] = near->pointUL.z;
	}
	
	slog("Entity position: x:%f ,y:%f,z:%f", ents->position.x, ents->position.y, ents->position.z);
	if ((quad->PuL.x + quad->PbR.x) / 2 <= ents->position.x)
	{
		// Indicates topLeftTree 
		if ((quad->PuL.y + quad->PbR.y) / 2 >= ents->position.y)
		{
			slog("Top Left");
			if (quad->UL == NULL)
				return 0.0;
			search_closest_quad(quad->UL, ents, near);
		}

		// Indicates botLeftTree 
		else
		{
			slog("Bottom Left");
			if (quad->BL == NULL)
				return 0.0;
			search_closest_quad(quad->BL, ents, near);
		}
	}
	else
	{
		// Indicates topRightTree 
		if ((quad->PuL.y + quad->PbR.y) / 2 >= ents->position.y)
		{
			slog("Top Right");
			if (quad->UR == NULL)
				return 0.0;
			search_closest_quad(quad->UR, ents, near);
		}

		// Indicates botRightTree 
		else
		{
			slog("Bottom Right");
			if (quad->BR == NULL)
				return 0.0;
			search_closest_quad(quad->BR, ents, near);
		}
	}
	
	return 0.0;
}
quadt* return_qtree(quadt* quad){
	quad = quad_terrain;
	return quad;
}
void* search(quadt *quad, Vector3D position){
	if (quad->n != NULL){
		//quad->n;
		slog("Point FOund x:%f y:%f", quad->n->pointUL.x, quad->n->pointUL.y);
		return;
	}
	if (!inBoundary(position, quad->PuL, quad->PbR)){
		slog("Not in bounds:");
		return;
	}
	//slog("Entity position: x:%f ,y:%f,z:%f", ents->position.x, ents->position.y, ents->position.z);
	if ((quad->PuL.x + quad->PbR.x) / 2 <= position.x)
	{
		// Indicates topLeftTree 
		if ((quad->PuL.y + quad->PbR.y) / 2 >= position.y)
		{
			slog("Top Left");
			if (quad->UL == NULL)
				return;
			search(quad->UL, position);
		}

		// Indicates botLeftTree 
		else
		{
			slog("Bottom Left");
			if (quad->BL == NULL)
				return;
			search(quad->BL, position);
		}
	}
	else
	{
		// Indicates topRightTree 
		if ((quad->PuL.y + quad->PbR.y) / 2 >= position.y)
		{
			slog("Top Right");
			if (quad->UR == NULL)
				return;
			search(quad->UR, position);
		}

		// Indicates botRightTree 
		else
		{
			slog("Bottom Right");
			if (quad->BR == NULL)
				return;
			search(quad->BR, position);
		}
	}

}

void update_skybox(Entity *self, Vector3D position){
	gfc_matrix_make_translation(self->EntMatx, vector3d(position.x,-position.y,position.z));
}