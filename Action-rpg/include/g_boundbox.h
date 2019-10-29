#ifndef __GF3D_ENTITY_H__
#define __GF3D_ENTITY_H__
#include "gf3d_model.h"
#include "gf3d_commands.h"
#include "gf3d_vgraphics.h"
#include "gf3d_obj_load.h"
typedef enum
{
    PS_Idle = 0,
	PS_Running = 1,
	PS_Attacking = 2,
	PS_HPSkill = 3,
	PS_Special = 4,
	PS_UsingItem = 5,
	PS_Hit = 6,
    PS_Dying = 7,
    PS_Dead = 8
}EntityState;

typedef struct BoundBox
{
	//Entity *self;
	Vector3D m_vecMin;
	Vector3D m_vecMax;
}BoundBox;

typedef struct Entity_S
{
    Uint8           _inuse;         /**<flag to keep track if this isntance is in use and should not be reassigned*/
    Model           *model;          /**<the 3d model for this entity*/
    Vector3D         position;       /**<position of the entity in 3d space*/
    Vector3D         velocity;       /**<velocity of the entity in 3d space*/
    Vector3D         acceleration;   /**<acceleration of the entity in 3d space*/
    Vector3D         rotation;       /**<yaw, pitch, and roll of the entity*/
    Vector3D         scale;          /**<*please default to 1,1,1*/
    EntityState      state;          /**<current state of the entity*/
	BoundBox		box;			//bounding box
    void (*think)(struct Entity_S* self);   /**<function called on entity think*/
    void (*update)(struct Entity_S* self);   /**<function called on entity update*/
    void (*touch)(struct Entity_S* self,struct Entity_S* other);   /**<function called on entity think*/
	void			(*move)(struct Entity_S* self);
	//void			(*attack)(struct Entity_S* self);
	void			(*special)(struct Entity_S* self,int n);
	void			(*hpSkill)(struct Entity_S* self);
	void			(*useItem)(struct Entity_S* self);
	void			(*target)(struct Entity_S* other);
    float           health;
    float           healthmax;
    float           defense;
	float           attack;
	float           mp;
    float           experience;
    float           level;
    float           otherStuff;
	int 			controling;	
	int				comboNum;
    void *data;                     /**<additional entity specific data*/
    
}Entity;

/**
 * @brief initializes the entity subsystem
 * @param entity_max maximum number of simultaneous entities you wish to support
 */
void gf3d_entity_manager_init(Uint32 entity_max);

/**
 * @brief get an empty entity from the system
 * @return NULL on out of space or a pointer to an entity otherwise
 */
Entity *gf3d_entity_new();

/**
 * @brief free an active entity
 * @param self the entity to free
 */
void gf3d_entity_free(Entity *self);

#endif