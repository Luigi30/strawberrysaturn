#include    <sgl.h>
#include    <sega_sys.h>
#include	<sega_gfs.h>
#include	<sega_per.h>

// Include file for main

static Uint16 pad_assign[]={
	PER_DGT_KU,
	PER_DGT_KD,
	PER_DGT_KR,
	PER_DGT_KL,
	PER_DGT_TA,
	PER_DGT_TB,
	PER_DGT_TC,
	PER_DGT_ST,
	PER_DGT_TX,
	PER_DGT_TY,
	PER_DGT_TZ,
	PER_DGT_TR,
	PER_DGT_TL,
};

//One extern per model.
extern PDATA 	PD_PLANE1;
extern PDATA	PD_SPR_HEART;
extern XPDATA	xpdata_ARROW;
extern XPDATA	xpdata_JAR;

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

#define PIC_OVERWATCHLOGO	0

void ss_main(void);
static void set_sprite(PICTURE *pcptr, Uint32 NbPicture);
static void disp_sprite(int SpriteNo, int rotation);

void TEXT_ClearLayer();

void GAME_IntroScreen();
void GAME_Begin();

void INTRO_DrawLogo(int rotationY);
void INTRO_DrawHeart(int x, int y);
