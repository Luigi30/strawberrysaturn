#include <sgl.h>
#include <sgl_cd.h>
#include <sega_sys.h>
#include <sega_per.h>
#include <ss_scrol.h>

#include "system.h" //memory addresses and junk
#include "camera.h"
#include "object.h"

#define PLAYER_Z_DISTANCE_FROM_CENTER (tankHullPos[Z] - (cameraBuffer.pos[Z] - toFIXED(CAMERA_Z_OFFSET)))

//One extern per model.
extern XPDATA	xpdata_ARROW;
extern XPDATA	xpdata_JAR;
extern XPDATA	xpdata_WALLS;
extern XPDATA	xpdata_LOGO;
extern XPDATA	xpdata_TANK_HULL;
extern XPDATA	xpdata_TANK_TURRET;
extern XPDATA	xpdata_BULLET;

//fedora.s2d
extern Uint8	cel_fedora;
extern Uint16	map_fedora;
extern Uint16	pal_fedora;

//ground.s2d
extern Uint8	cel_ground;
extern Uint16	map_ground;
extern Uint16	pal_ground;

//texture list
extern PICTURE  pic_overwatch[];
extern TEXTURE  tex_overwatch[];

typedef struct GAMESTATE {
	CAMERA		camera;
	
	FIXED		terrainPos[XYZ];
	WORLDOBJECT	player;
} GameState;

//One entry per sprite.
FIXED pos[][XYZS] =
{
    { toFIXED(0), toFIXED(0), toFIXED(170), toFIXED(1.0) },
};

//One entry per sprite.
SPR_ATTR attr[] =
{
    SPR_ATTRIBUTE(0, No_Palet, No_Gouraud, CL32KRGB | ECdis, sprNoflip),
};

GameState gameState;

void ss_main(void);
void VDP2_Init();

static void set_sprite(PICTURE *pcptr, Uint32 NbPicture);
static void disp_sprite(int SpriteNo, int rotation);

void TEXT_ClearLayer();

void GAME_IntroScreen();
void GAME_Begin();

void SCRL_Setup();

void WORLD_DrawArenaWalls();

VECTOR_3D GetDistanceFromCenterScreen(CAMERA *camera, FIXED *objectPosition);
