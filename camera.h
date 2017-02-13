#include <sgl.h>
#include "system.h"

//Camera offsets from the center of the world.
#define CAMERA_Y_OFFSET -400.0
#define CAMERA_Z_OFFSET -400.0

typedef struct cam {
	FIXED pos[XYZ];
	FIXED target[XYZ];
	ANGLE ang[XYZ];
} CAMERA;

void CAMERA_AdjustPositionForPlayerMovement(CAMERA *camera, FIXED *terrainPosition, FIXED *playerPosition);
VECTOR_3D GetDistanceFromCenterScreen(CAMERA *camera, FIXED *objectPosition);
void CAMERA_Init(CAMERA *camera);
