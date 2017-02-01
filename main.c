#include    "main.h"

#define		NUM_TEXTURES	4
#define     BACK_COL_ADR    (VDP2_VRAM_A1 + 0x1fffe)
#define     SystemWork      0x060ffc00              /* System Variable Address */
#define     SystemSize      (0x06100000-0x060ffc00) /* System Variable Size */

#define		MAX_FILE 512

extern PICTURE  pic_overwatch[];
extern TEXTURE  tex_overwatch[];
extern Uint32   _bstart, _bend;

int main(void)
{
	Uint8	*dst;
    Uint16  i;

	/* 1.Zero Set .bss Section */
    for(dst = (Uint8 *)&_bstart; dst < (Uint8 *)&_bend; dst++)
    {
		*dst = 0;
	}

    /* 2.SGL System Variable Clear */
    for(dst = (Uint8 *)SystemWork, i = 0; i < SystemSize; i++)
    {
		*dst = 0;
	}

	/* Application Call */
	ss_main();
}

void ss_main(void)
{
	//Initialize the Saturn with our texture list
    slInitSystem(TV_320x224, tex_overwatch, 1);
	slTVOff();

	//Load sprites into VRAM with DMA.
    set_sprite(pic_overwatch, NUM_TEXTURES);
    slColRAMMode(CRM16_1024);
    slBack1ColSet((void *)BACK_COL_ADR, 0);
	
	//Required setup stuff...
    slScrPosNbg1(toFIXED(-32.0), toFIXED(-36.0));
	slScrAutoDisp(NBG0ON | NBG1ON);

	slTVOn();
	
	GAME_IntroScreen();
	GAME_Begin();
}

static void set_sprite(PICTURE *pcptr, Uint32 NbPicture)
{
	TEXTURE *txptr;
 
	for(; NbPicture-- > 0; pcptr++){ 			//for each sprite in the list
		txptr = tex_overwatch + pcptr->texno;	//set the pointer to the array cell containing the pointer to the texture
		slDMACopy((void *)pcptr->pcsrc,			//then copy the texture into sprite VRAM.
			(void *)(SpriteVRAM + ((txptr->CGadr) << 3)),
			(Uint32)((txptr->Hsize * txptr->Vsize * 4) >> (pcptr->cmode)));
	}
}

static void disp_sprite(int SpriteNo, int rotation)
{
	//pos[SpriteNo] = entry in the positions table
	//&attr[SpriteNo].texno = texture ID number from &attr table
	//rotation = Z rotation of sprite graphic
	slDispSprite((FIXED *)pos[SpriteNo], (SPR_ATTR *)(&attr[SpriteNo].texno), DEGtoANG(rotation));
}

void INTRO_DrawLogo(int rotationY){
	static ANGLE	logoAng[XYZ] = { DEGtoANG(0.0), DEGtoANG(0.0), DEGtoANG(0.0) };
	static FIXED	logoPos[XYZ] = { toFIXED(0.0), toFIXED(0.0), toFIXED(220.0) };
	
	logoAng[Y] = DEGtoANG(rotationY);
	
	slPushMatrix();
	{
		slTranslate(logoPos[X], logoPos[Y], logoPos[Z]);
		slRotX(logoAng[X]);
		slRotY(logoAng[Y]);
		slRotZ(logoAng[Z]);
		slPutPolygon(&PD_PLANE1);
	}
	slPopMatrix();
}

void INTRO_DrawHeart(int x, int y){
	static ANGLE	ang[XYZ] = { DEGtoANG(0.0), DEGtoANG(0.0), DEGtoANG(0.0) };
	static FIXED	pos[XYZ];
	pos[X] = toFIXED(x);
	pos[Y] = toFIXED(y);
	pos[Z] = toFIXED(200.0);
	
	slPushMatrix();
	{
		slTranslate(pos[X], pos[Y], pos[Z]);
		slRotX(ang[X]);
		slRotY(ang[Y]);
		slRotZ(ang[Z]);
		slScale(toFIXED(0.2), toFIXED(0.2), toFIXED(0.2));
		slPutPolygon(&PD_SPR_HEART);
	}
	slPopMatrix();
}

void GAME_IntroScreen(){
	static FIXED	pos[16][XYZ];
	
	//Print some text.
    slPrint("McHanzo: The Game", slLocate(3,2));
	slPrint("Press Start", slLocate(3, 25));
	
	Uint16 framecounter = 0;
	Uint16 rotationDegrees = 0;
	
	int heartsX[4] = { 120, -70, -90, 10 };
	int heartsY[4] = { -200, -160, -110, -240 };
	int i;
	
	while(1)
    {
		int heart;
		
		rotationDegrees = (rotationDegrees + 1) % 360;
		//Display the proper sprite.
		//disp_sprite(PIC_OVERWATCHLOGO, rotationDegrees);
		
		INTRO_DrawLogo(rotationDegrees);
		
		if((Smpc_Peripheral[0].push & PER_DGT_ST) == 0) { //start button pressed?
			return; //end intro menu thing
		}
		
		slSynch();
	}
}

void GAME_Begin(){
	TEXT_ClearLayer();
	
	static ANGLE	arrowAng[XYZ] = { DEGtoANG(0.0), DEGtoANG(0.0), DEGtoANG(15.0) };
	static FIXED	arrowPos[XYZ] = { toFIXED(0.0), toFIXED(75.0), toFIXED(220.0) };
	
	int rotationY = 0;
	int frameCounter = 0;
	
	slPrint("www.itch.io/jam/strawberry-jam/", slLocate(3,26));
	
	while(1){
		
		frameCounter = (frameCounter + 1) % 60;
		
		if(frameCounter = 30){
			slPrint("Strawberry Jam!", slLocate(3,25));	
		}
		else if(frameCounter = 0){
			slPrint("               ", slLocate(3,25));	
		}
	
	/*
		if((Smpc_Peripheral[0].data & PER_DGT_KL) == 0){
			rotationY = (rotationY - 1) % 360;
		}
		if((Smpc_Peripheral[0].data & PER_DGT_KR) == 0){
			rotationY = (rotationY + 1) % 360;
		}
	*/
		rotationY = (rotationY + 1) % 360;
		arrowAng[Y] = DEGtoANG(rotationY);
	
		//Draw the little arrow thing
		slPushMatrix();
		{
			slTranslate(arrowPos[X], arrowPos[Y], arrowPos[Z]);
			slRotX(arrowAng[X]);
			slRotY(arrowAng[Y]);
			slRotZ(arrowAng[Z]);
			slPutPolygon((PDATA *)&xpdata_JAR);
		}
		slPopMatrix();
		
		slSynch();
	}
}

/* fills the screen with spaces */
void TEXT_ClearLayer()
{
     int i, j;

     for(i = 0; i < 30; i++)
     {
          for(j = 0; j < 40; j++)
          {
               slPrint(" ", slLocate(j, i));
          }
     }
}
