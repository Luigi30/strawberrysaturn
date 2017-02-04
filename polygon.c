#include	"sgl.h"

#include "polygons/skull.mdl"
#include "polygons/heart.mdl"
#include "polygons/arrow.mdl"
#include "polygons/jar.mdl"
#include "polygons/walls.mdl"

#define TEXTURE_OWLOGO		0
#define TEXTURE_HEART		1
#define TEXTURE_JARBLANK 	2
#define TEXTURE_JARSTRAW 	3

POINT point_PLANE1[] = {
        POStoFIXED(-100.0, -100.0, 0.0),
        POStoFIXED( 100.0, -100.0, 0.0),
        POStoFIXED( 100.0,  100.0, 0.0),
        POStoFIXED(-100.0,  100.0, 0.0),
};

POLYGON polygon_PLANE1[] = {
        NORMAL(0.0, 1.0, 0.0), VERTICES(0, 1, 2, 3),
};

ATTR attribute_PLANE1[] = {
        ATTRIBUTE(Dual_Plane, SORT_CEN, TEXTURE_OWLOGO, No_Palet, No_Gouraud, CL32KRGB | ECdis, sprNoflip, No_Option),
};

PDATA PD_PLANE1 = {
        point_PLANE1, sizeof(point_PLANE1) / sizeof(POINT),
        polygon_PLANE1, sizeof(polygon_PLANE1) / sizeof(POLYGON),
        attribute_PLANE1
};

/*************************/

POINT point_SPR_HEART[] = {
        POStoFIXED(-100.0, -100.0, 0.0),
        POStoFIXED( 100.0, -100.0, 0.0),
        POStoFIXED( 100.0,  100.0, 0.0),
        POStoFIXED(-100.0,  100.0, 0.0),
};

POLYGON polygon_SPR_HEART[] = {
        NORMAL(0.0, 1.0, 0.0), VERTICES(0, 1, 2, 3),
};

ATTR attribute_SPR_HEART[] = {
        ATTRIBUTE(Dual_Plane, SORT_CEN, TEXTURE_HEART, No_Palet, No_Gouraud, CL32KRGB | ECdis, sprNoflip, No_Option),
};

PDATA PD_SPR_HEART = {
        point_SPR_HEART, sizeof(point_SPR_HEART) / sizeof(POINT),
        polygon_SPR_HEART, sizeof(polygon_SPR_HEART) / sizeof(POLYGON),
        attribute_SPR_HEART
};
