#include "gf3d_model.h"
#include "gf3d_commands.h"
//#include "gf3d_vgraphics.h"
#include "gf3d_obj_load.h"
#include "g_entity.h"
#include <stdio.h>

typedef struct Node_S{
	Vector3D pointUL;
	Vector3D pointBL;
	Vector3D pointUR;
	Vector3D pointBR;
	int j;//indexes of points on array of vertices
	int i;
}Node;

typedef struct quadt_S{
	Vector2D PuL;
	Vector2D PbR;
	Node *n;
	struct quadt_S *UL;
	struct quadt_S *BL;
	struct quadt_S *UR;
	struct quadt_S *BR;
	bool isLeaf;
}quadt;
void init_floor_ent(Entity *self, int ctr, Entity *ents);
void init_wall_ent(Entity *self, int ctr, Entity *ents);
void update_skybox(Entity *self, Vector3D position);
float barryCentric(Vector3D p1, Vector3D p2, Vector3D p3, Vector2D pos);
float return_terrain_height(Entity* terr,float x, float y);
void load_terrain_data(char *filename);
void* search(quadt *quad, Vector3D position);
void create_quad(quadt *quad,Vector2D UL, Vector2D BR);
void insert_to_quad(quadt* quad, Vector2D PuL, Vector2D PbR, int Jindx, int Iindx);
bool inBoundary(Vector3D p, Vector2D PuL, Vector2D PbR);
float search_closest_quad(quadt *quad, Entity *ents, Node* near);
quadt* return_qtree(quadt* quad);