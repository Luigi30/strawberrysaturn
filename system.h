#ifndef SYSTEM_H
#define SYSTEM_H

#include <sgl.h>

#define     BACK_COL_ADR    (VDP2_VRAM_A1 + 0x1fffe)
#define     SystemWork 		(0x060ffc00)            /* System Variable Address */
#define     SystemSize      (0x06100000-0x060ffc00) /* System Variable Size */

//position and angle defines
#define 	FX_ZERO 	toFIXED(0.0)
#define 	DTA_ZERO 	DEGtoANG(0.0)
#define 	POS_ORIGIN 	{ FX_ZERO, FX_ZERO, FX_ZERO }
#define		SCALE_1		{ toFIXED(1.0), toFIXED(1.0), toFIXED(1.0) }
#define 	ANG_ZERO 	{ DEGtoANG(0.0), DEGtoANG(0.0), DEGtoANG(0.0) }

//RBG0 addresses
#define		RBG0RB_CEL_ADR		( VDP2_VRAM_A0 + 0x10000 )
#define		RBG0RB_MAP_ADR		( VDP2_VRAM_B0 + 0x02000 )
#define		RBG0RB_COL_OFFSET	0x00400

#define		RBG0RA_CEL_ADR		( VDP2_VRAM_A0 + 0x00000 )
#define		RBG0RA_MAP_ADR		( VDP2_VRAM_B0 + 0x00000 )
#define		RBG0RA_COL_OFFSET	0x00200

#define		RBG0_KTB_ADR		( VDP2_VRAM_A1 )
#define		RBG0_PRA_ADR		( VDP2_VRAM_A1 + 0x1fe00 )
#define		RBG0_PRB_ADR		( VDP2_VRAM_A1 + 0x1fe80 )

/* controls */
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

typedef struct Vector_3D {
	FIXED X, Y, Z;
} VECTOR_3D;

#endif
