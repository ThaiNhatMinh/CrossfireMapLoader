#pragma once

// WorldData flags.
#define WD_ORIGINALBSPALLOCED   (1<<0)  // m_pOriginalBsp is ours.
#define WD_WORLDBSPALLOCED      (1<<1)  // m_pWorldBsp is ours.

// World info flags for WorldBsp::m_WorldInfoFlags.
#define WIF_MOVEABLE            (1<<1)  // 1. WorldData::m_pWorldBsp is set so the world model
//    can be moved and rotated around.

#define WIF_MAINWORLD           (1<<2)  // 1. Preprocessor includes this in its poly stats
//    and shows detail stats for this world.
// 2. FLAG_GOTHRUWORLD is checked for world models
//    with this WorldBsp.

#define WIF_PHYSICSBSP          (1<<4)  // This is the physics BSP (only one of these).
#define WIF_VISBSP              (1<<5)  // This is the visibility BSP (only one of these).

// SharedTexture flags.  The m_RefCount member is shared by the flags and the refcount.
#define ST_TAGGED           0x8000  // Used by client when freeing unused textures.
#define ST_VALIDTEXTUREINFO 0x4000  // Used to determine if the width, height, format info on the texture is valid
#define ST_REFCOUNTMASK     0x3FFF

#define MAX_WORLDPOLY_VERTS 40


// When planes use special types, these are the indices.
#define PLANE_POSX      0
#define PLANE_NEGX      1
#define PLANE_POSY      2
#define PLANE_NEGY      3
#define PLANE_POSZ      4
#define PLANE_NEGZ      5
#define PLANE_GENERIC   6

// Node flags.
#define NF_IN           1
#define NF_OUT          2   

#define MAX_WORLDNAME_LEN       64

// Surface flags.
#define SURF_SOLID              (1<<0)      // Solid.
#define SURF_NONEXISTENT        (1<<1)      // Gets removed in preprocessor.
#define SURF_INVISIBLE          (1<<2)      // Don't draw.
#define SURF_TRANSPARENT        (1<<3)      // Translucent.
#define SURF_SKY                (1<<4)      // Sky portal.
#define SURF_BRIGHT             (1<<5)      // Fully bright.
#define SURF_FLATSHADE          (1<<6)      // Flat shade this poly.
#define SURF_LIGHTMAP           (1<<7)      // Lightmap this poly.
#define SURF_NOSUBDIV           (1<<8)      // Don't subdivide the poly.
#define SURF_HULLMAKER          (1<<9)      // Adds hulls to make PVS better for open areas.
#define SURF_DIRECTIONALLIGHT   (1<<11)     // This surface is only lit by the GlobalDirLight.
#define SURF_GOURAUDSHADE       (1<<12)     // Gouraud shade this poly.
#define SURF_PORTAL             (1<<13)     // This surface defines a portal that can be opened/closed.
#define SURF_PANNINGSKY         (1<<15)     // This surface has the panning sky overlaid on it.
#define SURF_PHYSICSBLOCKER     (1<<17)     // A poly used to block player movement
#define SURF_TERRAINOCCLUDER    (1<<18)     // Used for visibility calculations on terrain.
#define SURF_ADDITIVE           (1<<19)     // Add source and dest colors.
#define SURF_VISBLOCKER         (1<<21)     // Blocks off the visibility tree
#define SURF_NOTASTEP           (1<<22)     // Don't try to step up onto this polygon
#define SURF_MIRROR				(1<<23)		// This surface is a mirror

#define PT_STRING         0
#define PT_VECTOR         1
#define PT_COLOR          2
#define PT_REAL           3
#define PT_FLAGS          4
#define PT_BOOL           5
#define PT_LONGINT        6 
#define PT_ROTATION       7
#define NUM_PROPERTYTYPES 8

#define MAX_OBJ_NODE_LINKS		5
#define OBJ_NODE_LINK_ALWAYSVIS 4
#define MAX_WTNODE_CHILDREN		4
#define FRAMECODE_NOTINTREE		0xFFFFFFFF

enum WTObjType
{
	WTObj_DObject = 0,    // Bounding box object (DObject).
	WTObj_Light         // Light for accurate model lighting.
} ;

enum EAABBCorner {
	eAABB_NearTopLeft = 0,
	eAABB_NearTopRight = 1,
	eAABB_NearBottomLeft = 2,
	eAABB_NearBottomRight = 3,
	eAABB_FarTopLeft = 4,
	eAABB_FarTopRight = 5,
	eAABB_FarBottomLeft = 6,
	eAABB_FarBottomRight = 7,
	eAABB_None = 8,
};



// Different types of textures determined from their command string
enum ESharedTexType
{
	eSharedTexType_Single,							// No detail texture or environment map
	eSharedTexType_Detail,							// Detail Texture
	eSharedTexType_EnvMap,							// Environment map
	eSharedTexType_EnvMapAlpha,					// Environment map blended with the alpha channel
	eSharedTexType_EnvBumpMap,						// Environment map with an underlying bumpmap
	eSharedTexType_EnvBumpMap_NoFallback,		// Environment map with bumpmap, but falls back to normal texturing instead of environment map
	eSharedTexType_DOT3BumpMap,					// DOT3 bumpmap 
	eSharedTexType_DOT3EnvBumpMap,					// DOT3 bump map with environment map
	eSharedTexType_Effect
};

enum ELinkedTex
{
	eLinkedTex_EnvMap,			// The environment map of this texture
	eLinkedTex_Detail,			// The detail texture of this texture
	eLinkedTex_BumpMap,			// The bumpmap of this texture
	eLinkedTex_EffectTexture1,	// The effect texture 1 of this texture
	eLinkedTex_EffectTexture2,	// The effect texture 2 of this texture
	eLinkedTex_EffectTexture3,	// The effect texture 1 of this texture
	eLinkedTex_EffectTexture4,	// The effect texture 2 of this texture
};

//an enumeration describing the different forms of supported light attenuations
enum ELightAttenuationType
{
	eAttenuation_D3D,
	eAttenuation_Linear,
	eAttenuation_Quartic
};

typedef enum
{
	NOA_Objects = 0,          // Objects
	NOA_Lights,             // Static lights
	NUM_NODEOBJ_ARRAYS
} NodeObjArray;

/*!
Object flags.
*/
enum
{
	FLAG_VISIBLE = (1 << 0),

	//! Does this model cast shadows?
	FLAG_SHADOW = (1 << 1),

	//! Biases the Z towards the view so a sprite doesn't clip as much.
	FLAG_SPRITEBIAS = (1 << 2),

	//! Should this light cast shadows (slower)?
	FLAG_CASTSHADOWS = (1 << 3),

	//! Sprites only.
	FLAG_ROTATABLESPRITE = (1 << 3),

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	// minor fix for a common misspelling
#define FLAG_ROTATEABLESPRITE	FLAG_ROTATABLESPRITE
#endif // DOXYGEN_SHOULD_SKIP_THIS

	//! Particle systems only.
	/*!
	If this is set, the engine will update
	particles even when they're invisible.
	You should check FLAG_WASDRAWN
	on any particle systems you're iterating
	over so you don't update invisible ones.
	*/
	FLAG_UPDATEUNSEEN = (1 << 3),

	/*!
	The engine sets this if a particle system
	or PolyGrid was drawn.  You can use this to
	determine whether or not to do some expensive
	calculations on it.
	*/
	FLAG_WASDRAWN = (1 << 4),

	//! Disable Z read/write on sprite (good for lens flares).
	FLAG_SPRITE_NOZ = (1 << 4),

	//! Shrinks the sprite as the viewer gets nearer.
	FLAG_GLOWSPRITE = (1 << 5),

	//! Lights only - tells it to only light the world.
	FLAG_ONLYLIGHTWORLD = (1 << 5),

	//! For PolyGrids - says to only use the environment map (ignore main texture).
	FLAG_ENVIRONMENTMAPONLY = (1 << 5),

	//! Lights only - don't light backfacing polies.
	FLAG_DONTLIGHTBACKFACING = (1 << 6),

	/*!
	This is used for objects that are really close to the camera and would
	normally be clipped out of the view. PV weapons and effects attached
	to them are the best example. This tells the renderer to do some tricks
	to clip their near Z much closer to the eyepoint. This can be used for:
	- models
	- sprites without FLAG_ROTATEABLESPRITE
	- canvases
	*/
	FLAG_REALLYCLOSE = (1 << 6),

	//! Disables fog on WorldModels, Sprites, Particle Systems and Canvases only.
	FLAG_FOGDISABLE = (1 << 7),

	//! Lights only - tells it to only light objects (and not the world).
	FLAG_ONLYLIGHTOBJECTS = (1 << 7),

	/*!
	LT normally compresses the position and rotation info
	to reduce packet size.  Some things must be exact
	(like some WorldModels) so this will
	enlarge the packets for better accuracy.
	*/
	FLAG_FULLPOSITIONRES = (1 << 8),

	/*!
	Just use the object's color and global light scale.
	(Don't affect by area or by dynamic lights).
	*/
	FLAG_NOLIGHT = (1 << 9),

	//! Determines whether or not this object is paused and should not animate or update in a visible manner
	FLAG_PAUSED = (1 << 10),

	/*!
	Uses minimal network traffic to represent rotation
	(1 byte instead of 3, but only rotates around the Y axis).

	Mutually exclusive with FLAG_FULLPOSITIONRES (FLAG_FULLPOSITIONRES
	forces full uncompressed rotation distribution, and FLAG_YROTATION
	is ignored)
	*/
	FLAG_YROTATION = (1 << 11),

	//! Object is hit by raycasts.
	FLAG_RAYHIT = (1 << 12),

	//! Object can't go thru other solid objects.
	FLAG_SOLID = (1 << 13),

	//! Use simple box physics on this object (used for WorldModels and containers).
	FLAG_BOXPHYSICS = (1 << 14),

	//! This object is solid on the server and nonsolid on the client.
	FLAG_CLIENTNONSOLID = (1 << 15),

};

uint32 st_GetHashCode(const char *pString);