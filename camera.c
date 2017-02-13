#include "camera.h"

void CAMERA_AdjustPositionForPlayerMovement(CAMERA *camera, FIXED *terrainPosition, FIXED *playerPosition){
	VECTOR_3D distance = GetDistanceFromCenterScreen(camera, playerPosition);
	
	//If the player is more than 75 units away from the center of the screen in any direction,
	//adjust the camera's position and target so the player is exactly 75 units away.
	if(distance.Z > toFIXED(75.0))
	{
		FIXED overflowDistanceZ = distance.Z - toFIXED(75.0);
		camera->pos[Z] += overflowDistanceZ;
		camera->target[Z] += overflowDistanceZ;
		
		terrainPosition[Z] -= overflowDistanceZ;
	}
	
	if(distance.Z < toFIXED(-75.0))
	{
		FIXED overflowDistanceZ = distance.Z - toFIXED(-75.0);
		camera->pos[Z] += overflowDistanceZ;
		camera->target[Z] += overflowDistanceZ;		
		
		terrainPosition[Z] -= overflowDistanceZ;
	}
	
	if(distance.X > toFIXED(75.0))
	{
		FIXED overflowDistanceX = distance.X - toFIXED(75.0);
		camera->pos[X] += overflowDistanceX;
		camera->target[X] += overflowDistanceX;
		
		terrainPosition[X] -= overflowDistanceX;
	}
	
	if(distance.X < toFIXED(-75.0))
	{
		FIXED overflowDistanceX = distance.X - toFIXED(-75.0);
		camera->pos[X] += overflowDistanceX;
		camera->target[X] += overflowDistanceX;	

		terrainPosition[X] -= overflowDistanceX;
	}
}

VECTOR_3D GetDistanceFromCenterScreen(CAMERA *camera, FIXED *objectPosition){
	VECTOR_3D vector;
	
	vector.X = objectPosition[X] - camera->pos[X];
	vector.Y = objectPosition[Y] - (camera->pos[Y] - toFIXED(CAMERA_Y_OFFSET));
	vector.Z = objectPosition[Z] - (camera->pos[Z] - toFIXED(CAMERA_Z_OFFSET));
	
	return vector;
}

void CAMERA_Init(CAMERA *camera){
	camera->pos[X] = FX_ZERO;
	camera->pos[Y] = toFIXED(CAMERA_Y_OFFSET);
	camera->pos[Z] = toFIXED(CAMERA_Z_OFFSET);

	camera->ang[X] = DTA_ZERO;
	camera->ang[Y] = DTA_ZERO;
	camera->ang[Z] = DTA_ZERO;

	camera->target[X] = camera->target[Y] = camera->target[Z] = FX_ZERO;
}
