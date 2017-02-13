#include "object.h"

void OBJLIST_InsertHead(NODE_WORLDOBJECT **_list, const char *_name, FIXED *_pos, ANGLE *_angle, FIXED *_scale, PDATA *_polygons){
	/* Inserts a new node at the head of the object list with the provided payload. */
	
	NODE_WORLDOBJECT *node = NULL;
	node = malloc(sizeof(*node));
	
	strcpy(node->payload.name, _name);
	
	node->payload.position[X] = _pos[X];
	node->payload.position[Y] = _pos[Y];
	node->payload.position[Z] = _pos[Z];
	
	node->payload.angle[X] = _angle[X];
	node->payload.angle[Y] = _angle[Y];
	node->payload.angle[Z] = _angle[Z];
	
	node->payload.scale[X] = _scale[X];
	node->payload.scale[Y] = _scale[Y];
	node->payload.scale[Z] = _scale[Z];
	
	node->payload.polygons = _polygons;
	
	node->next = *_list;
	*_list = node;
}

NODE_WORLDOBJECT *OBJLIST_FindNodeByName(NODE_WORLDOBJECT *list, const char *name){
	/* 	Returns a pointer to the node with the specified name. 
		If no node is found, returns a null pointer. */
	
	NODE_WORLDOBJECT *current = list;
	
	while(current != NULL){
		if(strcmp(current->payload.name, name) == 0){
			return current;
		} else {
			current = current->next;
		}
	}
	return NULL;
}

NODE_WORLDOBJECT *OBJLIST_DeleteHead(NODE_WORLDOBJECT **list){
	//Removes the head node from the list and returns it.
	NODE_WORLDOBJECT *oldHeadNode = *list;
	*list = oldHeadNode->next;
	return oldHeadNode;
}

void OBJLIST_DeleteNodeByName(NODE_WORLDOBJECT *list, const char *name){	
	NODE_WORLDOBJECT *parent = list;
	
	//Case: the node we need is at the head of the list.
	if(strcmp(parent->payload.name, name) == 0){
		OBJLIST_DeleteHead(&list);
		return;
	}
	
	NODE_WORLDOBJECT *current = parent->next;
	
	//Case: the node we need is somewhere else in the list.
	while(current != NULL){
		if(strcmp(current->payload.name, name) == 0){
			parent->next = current->next; //remove this node from the list
			free(current); //free this node
			return; 		//done
		}
		
		current = current->next; //Didn't find it, keep traversing the list.
	}
}
