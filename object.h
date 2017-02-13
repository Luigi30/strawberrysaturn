#ifndef OBJECT_H
#define OBJECT_H

#include <sgl.h>
#include <stdlib.h>

//#define ADD_WORLDOBJECT(_LIST_, _NAME_, _POSX_, _POSY_, _POSZ_, _ANGX_, _ANGY_, _ANGZ_, _SCALEX_, _SCALEY_, _SCALEZ_, _PD_) { FIXED pos[XYZ] = { _POSX_, _POSY_, _POSZ_ }; ANGLE ang[XYZ] = { _ANGX_, _ANGY_, _ANGZ_ }; FIXED scale[XYZ] = { _SCALEX_, _SCALEY_, _SCALEZ_ }; OBJLIST_InsertHead(_LIST_, _NAME_, pos, ang, scale, (PDATA *)_PD_) };

/* This struct represents any object in the 3D world. */
typedef struct WorldObject {
	char				name[8];		//Reference name for lookup
	FIXED				position[XYZ];	//World coordinates
	ANGLE				angle[XYZ];		//Object rotation in 3 dimensions
	FIXED				scale[XYZ];		//Object scale in 3 dimensions
	PDATA				*polygons;		//Pointer to the object's PDATA
	struct WorldObject	*child;			//If the object has a child, it will be rendered using this object's
								//transformation matrix instead of the world transformation matrix.
} WORLDOBJECT;

/* The world object linked list is made up of these nodes. */
typedef struct Node_WorldObject {
	struct Node_WorldObject	*next;		//pointer to the next object in the list
	WORLDOBJECT				payload;	//the WORLDOBJECT that this node represents
} NODE_WORLDOBJECT;

void OBJLIST_InsertHead(NODE_WORLDOBJECT **_list, const char *_name, FIXED *_pos, ANGLE *_angle, FIXED *_scale, PDATA *_polygons);
NODE_WORLDOBJECT *OBJLIST_RemoveHead(NODE_WORLDOBJECT **_list);
NODE_WORLDOBJECT *OBJLIST_FindNodeByName(NODE_WORLDOBJECT *head, const char *name);
void OBJLIST_DeleteNodeByName(NODE_WORLDOBJECT *list, const char *name);

NODE_WORLDOBJECT *g_OBJLIST_WORLDOBJECTS; //Global list of world objects.

#endif
