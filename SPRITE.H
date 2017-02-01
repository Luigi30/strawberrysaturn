typedef enum {
	SPR_TYPE_SINGLE,
	SPR_TYPE_ANI_WRAM,
	SPR_TYPE_ANI_VRAM
} SpriteType;

typedef struct SPRITE {
	SpriteType type;
	char *name;
	FIXED pos[XYZS];
	ANGLE ang;
	Uint32 spriteID;
	// for animations
	Uint32 frame, nFrames;
	Uint8 *source, *target;
	Uint32 size;
	struct {
		Uint8 count, max;
	} frameskip;
} Sprite;
