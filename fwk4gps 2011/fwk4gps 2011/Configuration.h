#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

/* Header for Configuring the Framework
 *
 * Configuration.h
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

// Timing Factors
//
// units of time per sec returned by operating system
#define UNITS_PER_SEC   1000
// fps maximum - should be > flicker fusion threshold
#define FPS_MAX          200
// minimum elapsed time for updating to the next frame
#define MIN_ELAPSED_TIME UNITS_PER_SEC/FPS_MAX
// latency - keystroke time interval 
#define KEY_LATENCY      (UNITS_PER_SEC / 2)

// Default Window Dimensions
#define WND_WIDTH  800   // minimum window width
#define WND_HEIGHT 600   // minimum window height

// Sound Directory
#define AUDIO_DIRECTORY L"..\\..\\resources\\audio"

// Texture Directory
#define TEXTURE_DIRECTORY L"..\\..\\resources\\textures"

// NULL address
#define NULL 0

// Maximum string length throughout
#define MAX_DESC 255

// Sizes of context arrays
//
#define MAX_INTEGERS     40
#define MAX_FLOATS       10
#define MAX_VECTORS       4
#define MAX_BOOL_ARRAYS   3
#define MAX_INT_ARRAYS    4
#define MAX_STR_ARRAYS    8
#define MAX_ADAPTERS     10
#define MAX_MODES        50
#define MAX_P_FORMATS    20
#define MAX_KEYS        256
#define MAX_M_BUTTONS     3
#define MAX_C_BUTTONS   128
#define MAX_CONTROLLERS  10

typedef enum Integer {
    GF_AU_FREQ,
    GF_AU_MXVL,
    GF_AU_MNVL,
    GF_AU_CVOL,
    GF_DS_ACNT,
    GF_DS_MCNT,
    GF_DS_PCNT,
    GF_DS_ADPT,
    GF_DS_MODE,
    GF_DS_PIXL,
    GF_DS_RINW,
    GF_DS_WDTH,
    GF_DS_HGHT,
    GF_DS_MXLT,
    GF_DS_PXFW,
    GF_WN_WDTH,
    GF_WN_HGHT,
    GF_MS_DSPX,
    GF_MS_DSPY,
    GF_MS_ROTZ,
    GF_CT_INDX,
    GF_CT_TGRB,
    GF_CT_FLGS,
    GF_CT_CCNT,
    GF_CT_BCNT,
    GF_CT_POSX,
    GF_CT_POSY,
    GF_CT_DSPZ,
    GF_CT_ROTZ,
    GF_NO_ACTS,
    GF_NO_KEYS,
    GF_NO_SNDS,
    GT_XX_XXXX
} Integer;

typedef enum Float {
    GF_FR_NEAR,
    GF_FR_FAR,
    GF_FR_FOV,
    GF_FR_ASP
} Float;

typedef enum VectorId {
    GF_CA_POSN,
    GF_CA_HEAD,
    GF_CA_UP,
    GF_LT_BKGD
} VectorId;

typedef enum BoolArray {
    GF_KB_KEYS, // key states
    GF_MS_BTNS, // mouse button states
    GF_CT_BTNS  // controller button states
} BoolArray;

typedef enum IntArray {
    GF_TRGRBUTN,  // trigger buttons
    GF_KEYCODES   // key codes
} IntArray;

typedef enum StrArray {
    GF_ADPDESC, // adapter descriptions
    GF_MDEDESC, // mode descriptions
    GF_CTRDESC, // controller descriptions
    GF_CTBDESC, // controller button descriptions
    GF_ACTDESC, // action descriptions
    GF_KEYDESC, // key descriptions
    GF_SNDDESC, // sound descriptions
    GF_SNDFILE  // sound file names
} StrArray;

// mappable keys for configurable actions (do not initialize these constants)
//
typedef enum Key { 
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_0,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_SPACE,
    KEY_ENTER,
    KEY_UP   ,
    KEY_DOWN, 
    KEY_PGUP, 
    KEY_PGDN, 
    KEY_LEFT, 
    KEY_RIGHT,
    KEY_NUM1,
    KEY_NUM2,
    KEY_NUM3,
    KEY_NUM4,
    KEY_NUM5,
    KEY_NUM6,
    KEY_NUM7,
    KEY_NUM8,
    KEY_NUM9,
    KEY_ESCAPE,
	KEY_SEMICOLON,
	KEY_APOSTROPHE,
	KEY_O_BRACKET,
	KEY_C_BRACKET,
	KEY_BACKSLASH,
	KEY_COMMA,
	KEY_PERIOD,
	KEY_SLASH,
	KEY_TIMES,
	KEY_GRAVE,
	KEY_MINUS,
	KEY_UNDERSCORE,
	KEY_EQUALS,
	KEY_PLUS
} Key;

// mappable keys for configurable actions as listed in the user dialog
//
#define KEY_DESCRIPTIONS {\
	L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H", L"I", L"J", \
	L"K", L"L", L"M", L"N", L"O", L"P", L"Q", L"R", L"S", L"T", \
	L"U", L"V", L"W", L"X", L"Y", L"Z", \
	L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"0", \
	L"F1", L"F2", L"F3", L"F4", L"F5", L"F6", L"F7", \
	L"F8", L"F9", L"F10", L"F11", L"F12", \
	L"Space", L"Enter", L"Up", L"Down", L"PageUp", L"PageDown", \
	L"Left", L"Right", \
	L"NumPad 1", L"NumPad 2", L"NumPad 3", L"NumPad 4", L"NumPad 5", \
	L"NumPad 6", L"NumPad 7", L"NumPad 8", L"NumPad 9", \
	L"Escape", L";", L"'", L"[", L"]", L"\\", L",", \
	L".", L"/", L"*", L"`", L"-", L"_", L"=", L"+" \
}

// Actions
//
// to add an action
// - add the enumeration constant for the new action
// - add the friendly description of the new action
// - reset MAX_DESC above if necessary 
// - add the default key for the new action
//
// enumeration constants
typedef enum Action {
	MDL_ROLL_BOXES,
	MDL_SPIN_LEFT,
	MDL_SPIN_TOP,
    MDL_SPIN_SPOT_P,
    MDL_SPIN_SPOT_N,
    MDL_NEW_OBJECT,
    MDL_NEW_SOUND,
    MDL_NEW_LIGHT,
    MDL_DET_CHILD,
    MDL_ATT_CHILD,
	CAM_PITCH_UP,
	CAM_PITCH_DOWN,
	CAM_YAW_LEFT,
	CAM_YAW_RIGHT,
	CAM_ADVANCE,
	CAM_RETREAT,
    CAM_ROLL_LEFT,
    CAM_ROLL_RIGHT,
    CAM_STRAFE_LEFT,
    CAM_STRAFE_RIGHT,
    CAM_STRAFE_LEFT_ALT,
    CAM_STRAFE_RIGHT_ALT,
    CAM_FLY_UP,
    CAM_FLY_DOWN,
	AUD_BKGRD,
	AUD_IMPLS,
	AUD_VOLUME_INC,
	AUD_VOLUME_DEC,
	AUD_SPEED_UP,
	AUD_SLOW_DOWN,
	LIGHT_POINT,
	LIGHT_SPOT,
	LIGHT_DISTANT,
    LIGHT_CLONE,
	HUD_DISPLAY,
	HUD_LEFT,
	HUD_RIGHT,
	HUD_UP,
	HUD_DOWN,
    CAMERA_SELECT,
    ADJUST_PROJECTOR
} Action;

// user friendly descriptions of actions
//
#define ACTION_DESCRIPTIONS {\
	L"Roll Boxes",   \
	L"Spin Left Box",\
	L"Spin Top Box", \
    L"Spin Spot Light +", \
    L"Spin Spot Light -", \
    L"Clone Right Box", \
    L"Clone Left Sound", \
    L"Clone SpotLight", \
    L"Detach Child Box", \
    L"Attach Child Box", \
	L"Pitch Up",     \
	L"Pitch Down",   \
	L"Yaw Left",     \
	L"Yaw Right",    \
	L"Move Camera Forward",     \
	L"Move Camera Backward",    \
    L"Roll Camera Left", \
    L"Roll Camera Right", \
    L"Strafe Camera Left", \
    L"Strafe Camera Right", \
    L"Strafe Camera Left Alt", \
    L"Strafe Camera Right Alt", \
    L"Fly Camera Up", \
    L"Fly Camera Down", \
	L"Toggle Background Sound", \
	L"Explosion Sound", \
	L"Increase Volume", \
	L"Decrease Volume", \
	L"Speed Up Sound",  \
	L"Slow Down Sound", \
	L"Toggle Point Light",   \
	L"Toggle Spot Light",    \
	L"Toggle Distant Light", \
    L"Toggle Cloned Light", \
	L"Toggle Heads Up Display",      \
	L"Heads Up Display - Move Left", \
	L"Heads Up Display - Move Right",\
	L"Heads Up Display - Move Up",   \
	L"Heads Up Display - Move Down",  \
    L"Select Camera", \
    L"Adjust Projector" \
}

// initial mappings of actions to keys
//
#define ACTION_MAPPINGS {KEY_R, KEY_T, KEY_Y, KEY_SEMICOLON, KEY_BACKSLASH, KEY_U, KEY_I, KEY_O, KEY_P, KEY_V, \
    KEY_PGUP, KEY_PGDN, KEY_Z, KEY_C, KEY_W, KEY_S, KEY_Q, KEY_E, KEY_A, KEY_D, KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, \
	KEY_F3, KEY_F4, KEY_F6, KEY_F7, KEY_W, KEY_S, \
    KEY_J, KEY_K, KEY_L, KEY_H, \
    KEY_F, KEY_V,   \
	KEY_B, KEY_N, KEY_M, \
    KEY_X, KEY_1 }

// Mouse Buttons
//
typedef enum MouseButton {
    LEFT_BUTTON,
    RIGHT_BUTTON
} MouseButton;

// Controller Buttons
//
typedef enum CntlrButton {
    TRIGGER,
    BUTTON_1,
    BUTTON_2,
    BUTTON_3,
    BUTTON_4,
    BUTTON_5,
    BUTTON_6,
    BUTTON_7,
    BUTTON_8,
    BUTTON_9,
    BUTTON_10,
    NORTH,
    EAST,
    WEST,
    SOUTH
} CntrlrButton;

// Drawing Selection Categories
//
typedef enum Category {
    OPAQUE_OBJECT,
    TRANSLUCENT_OBJECT,
    TEST_COLOUR,
    OBJECT_TEXTURE,
    ALL_OBJECTS,
    HUD,
    SOUND
} Category;

// Graphics Primitive Types
//
typedef enum PrimitiveType {
    POINT_LIST     = 1,
    LINE_LIST      = 2,
    LINE_STRIP     = 3,
    TRIANGLE_LIST  = 4,
    TRIANGLE_STRIP = 5,
    TRIANGLE_FAN   = 6,
    SPHERE         = 7,
    CYLINDER       = 8,
    TORUS          = 9,
    TEAPOT         = 10
} PrimitiveType;

typedef enum VertexType {
    COLOURED_VERTEX = 1,
    LIT_VERTEX      = 2
} VertexType;

typedef enum BillboardType {
    SCREEN         = 1,
    VIEW_PLANE     = 2,
    VIEWPOINT      = 3,
    AXIAL          = 4
} BillboardType;

typedef enum RenderState {
    ALPHA_BLEND    = 1,
    Z_ENABLE       = 2
} RenderState;

// Sound Types
//
typedef enum SoundType {
    LOCAL_SOUND,  // stationary 3D
	MOBILE_SOUND, // mobile 3D
    GLOBAL_SOUND  // ambient
} SoundType;

// Light Types
//
typedef enum LightType {
    POINT_LIGHT,
    SPOT_LIGHT,
    DIRECTIONAL_LIGHT
} LightType;

// Coordinator Sounds
//
// to add a configurable sound
// - add its enumeration constant
// - add its description
// - add the default filename for the sound
// - reset MAX_DESC above if necessary 
//
typedef enum ModelSound {
	SND_BKGRD,
	SND_DISCR,
	SND_OBJECT,
	SND_LOCAL_L,
	SND_LOCAL_R
} ModelSound;

// friendly descriptions of configurable sounds as listed in the user dialog 
//
#define SOUND_DESCRIPTIONS {\
	L"Background",\
	L"Discrete",\
	L"Object",\
	L"Local Left",\
	L"Local Right"\
}

// initial selection of configurable sounds 
//
// include the authors name for CCS+ accreditation
//
#define SOUND_MAPPINGS {\
	L"Crickets (by reinsamba) .xwma",\
	L"Gong (by reinsamba) .xwma",\
	L"Goat (by reinsamba) .xwma",\
	L"Street_accordeonist (by reinsamba) .xwma",\
	L"Fortaleza election campaign (by reinsamba) .xwma",\
}

// Dialog options
//
// default controller
#define NO_CONTROLLER_DESC L"No Controller"
#define TRIGGER_DESC       L""

// Text alignment flags
//
#define TEXT_LEFT    1
#define TEXT_RIGHT   2
#define TEXT_CENTER  4
#define TEXT_TOP     8
#define TEXT_BOTTOM 16
#define TEXT_MIDDLE 32
#define TEXT_DEFAULT TEXT_LEFT | TEXT_TOP

// Text display colour [0,255]
//
#define TEXT_R 0
#define TEXT_G 0
#define TEXT_B 0
#define TEXT_A 255

// texture parameters
//
#define TEXTURE_WIDTH  256
#define TEXTURE_HEIGHT 256
#define DEPTH_MAP_SIZE 1024

// Texture filtering flags
//
#define MAX_STAGES           2
#define TEX_MIN_POINT        1
#define TEX_MIN_LINEAR       2
#define TEX_MIN_ANISOTROPIC  4
#define TEX_MAG_POINT        8
#define TEX_MAG_LINEAR      16
#define TEX_MAG_ANISOTROPIC 32
#define TEX_MIPMAP          64
#define TEX_WRAP_U         128
#define TEX_CLAMP_U        256
#define TEX_BORDER_U       512
#define TEX_MIRROR_U      1024
#define TEX_WRAP_V        2048
#define TEX_CLAMP_V       4096
#define TEX_BORDER_V      8192
#define TEX_MIRROR_V     16384
#define TEX_PROJECTED    32768
#define TEX_DEFAULT TEX_MIN_LINEAR | TEX_MAG_LINEAR | TEX_MIPMAP | TEX_WRAP_U | TEX_WRAP_V

// sprite modulation colour for the HUD texture [0,255]
//
#define SPRITE_R 255
#define SPRITE_G 255
#define SPRITE_B 255

// projection frustum properties
//
#define NEAR_CLIPPING 1.0f   // in coordinator units
#define FAR_CLIPPING  2000.f // in coordinator units 
#define FIELD_OF_VIEW 0.9f   // in radians

// texture projection frustum properties
//
#define TEX_NEAR_CLIPPING 0.09f  // in coordinator units
#define TEX_FAR_CLIPPING  500.f  // in coordinator units 
#define TEX_FIELD_OF_VIEW 0.9f   // in radians

// Rectangle
//
//-------------------------------- Rect ---------------------------------------
//
typedef struct Rect {
    int topLeftX;
    int topLeftY;
    int bottomRightX;
    int bottomRightY;
    Rect(int tlx = 0, int tly = 0, int brx = 1, int bry = 1) : 
     topLeftX(tlx), topLeftY(tly), bottomRightX(brx), bottomRightY(bry) {}
} Rect;

typedef struct RelRect {
    float topLeftX;
    float topLeftY;
    float bottomRightX;
    float bottomRightY;
    RelRect(float tlx = 0.0f, float tly = 0.0f, float brx = 1.0f, float bry = 1.0f) : 
     topLeftX(tlx), topLeftY(tly), bottomRightX(brx), bottomRightY(bry) {}
    Rect absolute(int w, int h) { 
        return Rect((int)(topLeftX * w), (int)(topLeftY * h), 
         (int)(bottomRightX * w), (int)(bottomRightY * h)); 
    }
} RelRect;

// HUD font Flags
//
#define HUD_ALPHA 1

// HUD Text Symbols
#define TEXT_ON  L"+" // symbol for on
#define TEXT_OFF L"o" // symbol for off


// sound parameters
//
#define INTERIOR_CONE               0.67f // in radians
#define SOUND_DISTANCE_FACTOR       1.0f  // metres per coordinator unit
#define ATTENUATION_DISTANCE_FACTOR 10    // where attenuation begins

#endif


