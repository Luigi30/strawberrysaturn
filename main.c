#include    "main.h"

#define		NUM_TEXTURES	1

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
	//Setup globals
	g_OBJLIST_WORLDOBJECTS = NULL;
	
	//Initialize the Saturn with our texture list.
    slInitSystem(TV_320x224, tex_overwatch, 1);
	
	slTVOff();
	VDP2_Init();
	slTVOn();
	GAME_IntroScreen();
	GAME_Begin();
}

void VDP2_Init(){
	//Load sprites into VRAM with DMA.
	
    set_sprite(pic_overwatch, NUM_TEXTURES);
    slColRAMMode(CRM16_1024);
	
	//Setup RBG0
	slRparaInitSet((void *)RBG0_PRA_ADR);
	slMakeKtable((void *)RBG0_KTB_ADR); //coefficient table for perspective transformations
	slCharRbg0(COL_TYPE_256, CHAR_SIZE_2x2); //256 colors, 2x2 chars. Set up according to .map file
	slPageRbg0((void *)RBG0RA_CEL_ADR, (void *)RBG0RA_COL_OFFSET, PNB_1WORD|CN_10BIT); //set up according to .map file
	slPlaneRA(PL_SIZE_1x1); //Map can be 1x1, 1x2, 2x1, or 2x2 pages
	sl1MapRA((void *)RBG0RA_MAP_ADR);
	slOverRA(0); //overflow mode - see manual
	slKtableRA((void *)RBG0_KTB_ADR, K_FIX | K_DOT | K_2WORD | K_ON);
	
	Cel2VRAM(&cel_ground, (void *)RBG0RA_CEL_ADR, 16896);
	Map2VRAM(&map_ground, (void *)RBG0RA_MAP_ADR, 128/2, 128/2, 1, 0); //fedora_map contains a 32x32 graphic.
	Pal2CRAM(&pal_ground, (void *)VDP2_COLRAM+RBG0RA_COL_OFFSET, 256);
	
	slRparaMode(K_CHANGE);
	slBack1ColSet((void *)BACK_COL_ADR, CD_DarkBlue);
	
	slScrAutoDisp(NBG0ON);
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

void WORLD_DrawObjects(NODE_WORLDOBJECT *head){
	int drawn = 0;
	char strDrawn[32];
	
	NODE_WORLDOBJECT *current = head;
	
	while(current != NULL)
	{	
		drawn++;
		sprintf(strDrawn, "OBJ: %d", drawn);
		slPrint(strDrawn, slLocate(3,24));
		
		slPushMatrix();
		{
			slTranslate(current->payload.position[X], current->payload.position[Y], current->payload.position[Z]);
			slRotX(current->payload.angle[X]);
			slRotY(current->payload.angle[Y]);
			slRotZ(current->payload.angle[Z]);
			slScale(current->payload.scale[X], current->payload.scale[Y], current->payload.scale[Z]);
			slPutPolygon(current->payload.polygons);
		}
		slPopMatrix();
		
		current = current->next;
	}
	
}

void GAME_IntroScreen(){	
	Uint16 rotationDegrees = 0;
	
	slPrint("Tank Game", slLocate(3,2));
	slPrint("Press Start", slLocate(3, 25));
	
	ANGLE logoAng[XYZ] = { DEGtoANG(0.0), DEGtoANG(0.0), DEGtoANG(0.0) };
	FIXED logoPos[XYZ] = { toFIXED(0.0), toFIXED(0.0), toFIXED(220.0) };
	FIXED logoScale[XYZ] = SCALE_1;
	OBJLIST_InsertHead(&g_OBJLIST_WORLDOBJECTS, "LOGO", logoPos, logoAng, logoScale, (PDATA *)&xpdata_LOGO);
	
	while(1)
    {	
		rotationDegrees = (rotationDegrees + 1) % 360;
		
		//TODO: add spinning tank instead of stolen graphic!
		OBJLIST_FindNodeByName(g_OBJLIST_WORLDOBJECTS, "LOGO")->payload.angle[Y] = DEGtoANG(rotationDegrees);
		WORLD_DrawObjects(g_OBJLIST_WORLDOBJECTS);
		
		if((Smpc_Peripheral[0].push & PER_DGT_ST) == 0) { //start button pressed?
			return; //end intro menu thing
		}

		slPushMatrix();
		{	
			slCurRpara(RA);
			slUnitMatrix(CURRENT);
			slTranslate(FX_ZERO, FX_ZERO, toFIXED(400.0));
			slRotX(DEGtoANG(-30.0));
			slScrMatConv();
			slScrMatSet();
		}
		slPopMatrix();
		
		slSynch();
	}
}

/*
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
*/

void DEBUG_PrintCoordinates(FIXED *pos, ANGLE *ang) {
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

void WORLD_DrawArenaWalls(){
	slTranslate(FX_ZERO, toFIXED(25.0), FX_ZERO);
	slRotX(DTA_ZERO);
	slRotY(DTA_ZERO);
	slRotZ(DTA_ZERO);
	slScale(toFIXED(1.5), toFIXED(1.0), toFIXED(1.25));
	slPutPolygon((PDATA *)&xpdata_WALLS);
}

void GAME_Begin(){
	OBJLIST_DeleteHead(&g_OBJLIST_WORLDOBJECTS);
	
	TEXT_ClearLayer();
	
	CAMERA_Init(&gameState.camera);
	
	//rotX needs to match the camera's viewing angle.
	//for now, assume the points are coplanar since the camera only moves in 2 dimensions
	ANGLE terrainRotX = slAtan(toFIXED(-400.0), toFIXED(-400.0));

	static FIXED tankHullPos[XYZ] = POS_ORIGIN;
	static ANGLE tankHullAng[XYZ] = ANG_ZERO;
	
	static FIXED turretPos[XYZ] = POS_ORIGIN;
	static ANGLE turretAng[XYZ] = ANG_ZERO;

	gameState.terrainPos[X] = gameState.terrainPos[Y] = gameState.terrainPos[Z] = FX_ZERO;
	
	FIXED forwardSpeed = toFIXED(2.0);
	
	int hullRotationY = 0;
	int turretRotationY = 0;
	int frameCounter = 0;
	
	slPrint("Strawberry Jam!", slLocate(3,25));
	slPrint("www.itch.io/jam/strawberry-jam/", slLocate(3,26));	
	
	//SCRL_Setup();
	slScrAutoDisp(NBG0ON | RBG0ON);
	
	//Add arena walls to object list
	{
		ANGLE ang[XYZ] = { DEGtoANG(0.0), DEGtoANG(0.0), DEGtoANG(0.0) };
		FIXED pos[XYZ] = { toFIXED(0.0), toFIXED(25.0), toFIXED(0.0) };
		FIXED scale[XYZ] = { toFIXED(1.5), toFIXED(1.0), toFIXED(1.25) };
		OBJLIST_InsertHead(&g_OBJLIST_WORLDOBJECTS, "WALLS", pos, ang, scale, (PDATA *)&xpdata_WALLS);
	}
	
	while(1){		
		frameCounter = (frameCounter + 1) % 60;
		
		slUnitMatrix(CURRENT); //Reset the master transformation matrix.
		
		//Adjust the camera position if the player is too far away
		CAMERA_AdjustPositionForPlayerMovement(&gameState.camera, gameState.terrainPos, tankHullPos);
		
		//Orient the camera.
		slLookAt(gameState.camera.pos, gameState.camera.target, gameState.camera.ang[Z]);
		
		DEBUG_PrintCoordinates(tankHullPos, tankHullAng);
	
		//Player's forward vector with magnitude of forwardSpeed.
		FIXED vec_Forward[XYZ] = { slMulFX(forwardSpeed, slSin(tankHullAng[Y])), toFIXED(0.0), slMulFX(forwardSpeed,slCos(tankHullAng[Y])) };

		//Hull rotation
		if((Smpc_Peripheral[0].data & PER_DGT_KL) == 0){
			hullRotationY = (hullRotationY - 2) % 360;
		}
		if((Smpc_Peripheral[0].data & PER_DGT_KR) == 0){
			hullRotationY = (hullRotationY + 2) % 360;
		}
		if((Smpc_Peripheral[0].data & PER_DGT_KU) == 0){
			tankHullPos[X] = tankHullPos[X] + vec_Forward[X];
			tankHullPos[Z] = tankHullPos[Z] + vec_Forward[Z];
		}
		if((Smpc_Peripheral[0].data & PER_DGT_KD) == 0){
			tankHullPos[X] = tankHullPos[X] - vec_Forward[X];
			tankHullPos[Z] = tankHullPos[Z] - vec_Forward[Z];
		}
		
		/* turret rotation */
		if((Smpc_Peripheral[0].data & PER_DGT_TL) == 0){
			turretRotationY = (turretRotationY-2) % 360;
		}
		if((Smpc_Peripheral[0].data & PER_DGT_TR) == 0){
			turretRotationY = (turretRotationY+2) % 360;
		}
		
		/* player tank shell */
		if((Smpc_Peripheral[0].data & PER_DGT_TA) == 0){
			slPushMatrix();
			{
				slTranslate(tankHullPos[X] + slMulFX(toFIXED(75.0), slSin(tankHullAng[Y])), 
							toFIXED(-20.0),
							tankHullPos[Z] + slMulFX(toFIXED(75.0), slCos(tankHullAng[Y])));
				slRotX(turretAng[X]);
				slRotY(DEGtoANG(hullRotationY + turretRotationY));
				slRotZ(turretAng[Z]);
				slScale(toFIXED(0.20), toFIXED(0.20), toFIXED(0.20));
				slPutPolygon((PDATA *)&xpdata_BULLET);
			}
			slPopMatrix();
		}
	
		//draw tank
		tankHullAng[Y] = DEGtoANG(hullRotationY);
		turretAng[Y] = DEGtoANG(turretRotationY);
		
		//hull's translation matrix
		slPushMatrix();
		{
			slTranslate(tankHullPos[X], tankHullPos[Y], tankHullPos[Z]);
			slRotX(tankHullAng[X]);
			slRotY(tankHullAng[Y]);
			slRotZ(tankHullAng[Z]);
			slScale(toFIXED(0.5), toFIXED(0.5), toFIXED(0.5));
			slPutPolygon((PDATA *)&xpdata_TANK_HULL);
		}
		
		slPushMatrix(); //turret's translation matrix is a child of the hull's translation matrix
		{
			slTranslate(turretPos[X], turretPos[Y], turretPos[Z]);
			slRotX(turretAng[X]);
			slRotY(turretAng[Y]);
			slRotZ(turretAng[Z]);
			slScale(toFIXED(1), toFIXED(1), toFIXED(1));
			slPutPolygon((PDATA *)&xpdata_TANK_TURRET);	
		}
		
		slPopMatrix(); //done with child matrix
		slPopMatrix(); //done with parent matrix
		
		//Ground layer - RBG0
		slPushUnitMatrix();
		{
			slCurRpara(RA);
			slTranslate(gameState.terrainPos[X], toFIXED(200), toFIXED(400));
			slRotX(-terrainRotX);
			slScrMatConv();
			slScrMatSet();
		}
		slPopMatrix();
		
		WORLD_DrawObjects(g_OBJLIST_WORLDOBJECTS);
		
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
