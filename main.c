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

void VDP2_Init(){
	//Load sprites into VRAM with DMA.
    set_sprite(pic_overwatch, NUM_TEXTURES);
    slColRAMMode(CRM16_1024);
    slBack1ColSet((void *)BACK_COL_ADR, 0);
    slScrPosNbg1(toFIXED(-32.0), toFIXED(-36.0));
	slScrAutoDisp(NBG0ON | NBG1ON);
}

void ss_main(void)
{
	//Initialize the Saturn with our texture list.
    slInitSystem(TV_320x224, tex_overwatch, 1);
	slTVOff();
	VDP2_Init();
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
	static ANGLE logoAng[XYZ] = { DEGtoANG(0.0), DEGtoANG(0.0), DEGtoANG(0.0) };
	static FIXED logoPos[XYZ] = { toFIXED(0.0), toFIXED(0.0), toFIXED(220.0) };
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
	Uint16 rotationDegrees = 0;
	
	slPrint("Tank Game", slLocate(3,2));
	slPrint("Press Start", slLocate(3, 25));
	
	while(1)
    {	
		rotationDegrees = (rotationDegrees + 1) % 360;
		
		//TODO: add spinning tank instead of stolen graphic!
		INTRO_DrawLogo(rotationDegrees);
		
		if((Smpc_Peripheral[0].push & PER_DGT_ST) == 0) { //start button pressed?
			return; //end intro menu thing
		}
		
		slSynch();
	}
}

void SCRL_Setup(){
	//Set up the scroll
	slColRAMMode(CRM16_1024); //keep CRM16_1024 usually
	slPlaneNbg1(PL_SIZE_1x1); //Map can be 1x1, 1x2, 2x1, or 2x2 pages
	slCharNbg1(COL_TYPE_256, CHAR_SIZE_2x2); //256 colors, 2x2 chars. Set up according to .map file
	slPageNbg1((void *)VDP2_VRAM_A0, (void *)0x200, PNB_1WORD|CN_10BIT); //set up according to .map file
	slMapNbg1((void *)VDP2_VRAM_B0, (void *)VDP2_VRAM_B0, (void *)VDP2_VRAM_B0, (void *)VDP2_VRAM_B0); //set layer ptrs
	slScrPosNbg1(toFIXED(0), toFIXED(0)); //position the scroll layer
	slZoomNbg1(toFIXED(0.5), toFIXED(0.5)); //X and Y parameters are RECIPROCAL of desired zoom level
	
	Cel2VRAM(&cel_fedora, (void *)VDP2_VRAM_A0, 59136);
	
	//Map2VRAM(Uint16 *start_of_tile_data , Uint16 *start_of_map_data, Uint16 map_width, Uint16 map_height, Uint16 palette_used, Uint16 map_offset);
	Map2VRAM(&map_fedora, (void *)VDP2_VRAM_B0, 128/2, 128/2, 1, 0); //fedora_map contains a 128x128 graphic.
	Pal2CRAM(&pal_fedora, (void *)(VDP2_COLRAM+0x200), 256);
	
	slScrAutoDisp(NBG0ON | NBG1ON);
}

void PRINT_PrintCoordinates(FIXED *pos, ANGLE *ang) {
	//Draws the object's position and angle on the screen.
	
	slPrint("POSITION", slLocate(3,1));
	slPrintFX(pos[X], slLocate(3,2));
	slPrintFX(pos[Y], slLocate(3,3));
	slPrintFX(pos[Z], slLocate(3,4));
	
	slPrint("ANGLE", slLocate(17,1));
	slPrintFX(slAng2FX(ang[X]), slLocate(13,2));
	slPrintFX(slAng2FX(ang[Y]), slLocate(13,3));
	slPrintFX(slAng2FX(ang[Z]), slLocate(13,4));
}

void GAME_Begin(){
	TEXT_ClearLayer();
	
	//Set up the camera.
	static CAMERA cameraBuffer;
	{
		cameraBuffer.pos[X] = FX_ZERO;
		cameraBuffer.pos[Y] = toFIXED(-400.0);
		cameraBuffer.pos[Z] = toFIXED(-400.0);
		
		cameraBuffer.ang[X] = ZERO[X];
		cameraBuffer.ang[Y] = ZERO[Y];
		cameraBuffer.ang[Z] = ZERO[Z];
		
		cameraBuffer.target[X] = cameraBuffer.target[Y] = cameraBuffer.target[Z] = FX_ZERO;
	}
	
	static ANGLE arrowAng[XYZ] = { DEGtoANG(0.0), DEGtoANG(0.0), DEGtoANG(0.0) };
	static FIXED arrowPos[XYZ] = { toFIXED(0.0), toFIXED(0.0), toFIXED(0.0) };
	
	//FIXED fedora_pos_X = SIPOSX;
	FIXED forwardSpeed = toFIXED(2.0);
	
	int rotationY = 0;
	int frameCounter = 0;
	
	slPrint("Strawberry Jam!", slLocate(3,25));
	slPrint("www.itch.io/jam/strawberry-jam/", slLocate(3,26));	
	
	//SCRL_Setup();
	
	while(1){
		frameCounter = (frameCounter + 1) % 60;
		
		slUnitMatrix(CURRENT); //Reset the master transformation matrix.
		
		//Orient the camera.
		slLookAt(cameraBuffer.pos, cameraBuffer.target, cameraBuffer.ang[Z]);
		
		PRINT_PrintCoordinates(arrowPos, arrowAng);
		
		//slScrPosNbg1(fedora_pos_X, toFIXED(0));
		//fedora_pos_X += POSX_UP;
	
		//Player's forward vector with magnitude of forwardSpeed.
		FIXED vec_Forward[XYZ] = { slMulFX(forwardSpeed, slSin(arrowAng[Y])), toFIXED(0.0), slMulFX(forwardSpeed,slCos(arrowAng[Y])) };

		//Handle inputs from control pad 0
		if((Smpc_Peripheral[0].data & PER_DGT_KL) == 0){
			rotationY = (rotationY - 2) % 360;
		}
		if((Smpc_Peripheral[0].data & PER_DGT_KR) == 0){
			rotationY = (rotationY + 2) % 360;
		}
		if((Smpc_Peripheral[0].data & PER_DGT_KU) == 0){
			arrowPos[X] = arrowPos[X] + vec_Forward[X];
			arrowPos[Z] = arrowPos[Z] + vec_Forward[Z];
		}
		if((Smpc_Peripheral[0].data & PER_DGT_KD) == 0){
			arrowPos[X] = arrowPos[X] - vec_Forward[X];
			arrowPos[Z] = arrowPos[Z] - vec_Forward[Z];
		}
	
		//Draw the cute little arrow thing that's a placeholder for the player.
		arrowAng[Y] = DEGtoANG(rotationY);
		slPushMatrix();
		{
			slTranslate(arrowPos[X], arrowPos[Y], arrowPos[Z]);
			slRotX(arrowAng[X]);
			slRotY(arrowAng[Y]);
			slRotZ(arrowAng[Z]);
			slScale(toFIXED(0.5), toFIXED(0.5), toFIXED(0.5));
			slPutPolygon((PDATA *)&xpdata_ARROW);
		}
		slPopMatrix();
		
		//Draw the arena walls.
		slPushMatrix();
		{
			slTranslate(ORIGIN[X], toFIXED(25.0), ORIGIN[Z]);
			slRotX(ZERO[X]);
			slRotY(ZERO[Y]);
			slRotZ(ZERO[Z]);
			slScale(toFIXED(1.5), toFIXED(1.0), toFIXED(1.25));
			slPutPolygon((PDATA *)&xpdata_WALLS);
		}
		slPopMatrix();
		
		//End frame.
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
