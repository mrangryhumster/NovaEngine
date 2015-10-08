#ifndef ERENDERERENUMS_H
#define ERENDERERENUMS_H


namespace novaengine
{
namespace renderer
{

enum E_RENDERER_TYPE
{
    ERT_NULL        = 0x0,
    ERT_OPENGL      = 0x1
};
//------------------------------------------
enum E_CLEAR_FLAGS
{
    ECF_COLOR_BUFFER = 0x1,
    ECF_DEPTH_BUFFER = 0x2,
};
//------------------------------------------
enum E_RENDERER_FEATURE
{
    ERF_RENDER_GRAPHICS         = 0x1,
    ERF_VBO                     = 0x2,
    ERF_RENDER_TO_TEXTURE       = 0x3,
    ERF_SHADERS_1_0             = 0x4,
    ERF_SHADERS_2_0             = 0x5,
    ERF_SHADERS_3_0             = 0x6
};
//------------------------------------------
enum E_RENDER_STATE
{
    ERS_ENABLE_DEPTH_TEST       = 0,
    ERS_ENABLE_DEPTH_WRITE         ,
    ERS_DEPTH_TEST_MODE            ,
    ERS_ENABLE_BLENDING            ,
    ERS_BLENDING_MODE              ,
    ERS_ENABLE_TEXTURES_2D         ,
    ERS_ENABLE_WIREFRAME           ,
    ERS_ENABLE_CULL_FACE           ,
    ERS_CULL_FACE_MODE             ,
    ERS_ENABLE_ALPHA_TEST          ,
    ERS_ALPHA_TEST_THRESHOLD       ,
    ERS_ENABLE_FOG                 ,
    ERS_FOG_MODE                   ,
    ERS_FOG_START_DISTANCE         ,
    ERS_FOG_END_DISTANCE           ,
    ERS_FOG_DENSITY                ,
    ERS_LINE_WIDTH                 ,
    ERS_POINT_SIZE                 ,
    ERS_LAST_STATE                 ,
};
//------------------------------------------
enum E_DEPTH_TEST_MODE
{
    EDTM_NEVER      =   0x00,
    EDTM_LESS       =   0x01,
    EDTM_EQUAL      =   0x02,
    EDTM_LEQUAL     =   0x03,
    EDTM_GREATER    =   0x04,
    EDTM_NOTEQUAL   =   0x05,
    EDTM_GEQUAL     =   0x06,
    EDTM_ALWAYS     =   0x07
};
//------------------------------------------
enum E_CULL_FACE_MODE
{
    ECFM_FRONT = 0x1,
    ECFM_BACK  = 0x2,
    ECFM_ALL   = 0x3,
};
//------------------------------------------
#define NE_BLENDMODE(MODE_1,MODE_2) (MODE_1 << 16) | (MODE_2)
enum E_BLENDING_MODE
{
    EBM_ZERO                    = 0x0,
    EBM_ONE                     = 0x1,
    EBM_SRC_COLOR               = 0x2,
    EBM_ONE_MINUS_SRC_COLOR     = 0x3,
    EBM_DST_COLOR               = 0x4,
    EBM_ONE_MINUS_DST_COLOR     = 0x5,
    EBM_SRC_ALPHA               = 0x6,
    EBM_ONE_MINUS_SRC_ALPHA     = 0x7,
    EBM_DST_ALPHA               = 0x8,
    EBM_ONE_MINUS_DST_ALPHA     = 0x9,
};
//------------------------------------------
enum E_FOG_MODE
{
    EFM_FASTEST = 0x0,
    EFM_NICEST  = 0x1,
};

//------------------------------------------
enum E_MATRIX_TYPE
{
    EMT_PROJECTION = 0x0,
    EMT_VIEW       = 0x1,
    EMT_MODEL      = 0x2,
    EMT_TEXTURE    = 0x3,
};
//------------------------------------------
enum E_ATTRIB_TYPE
{
    EAT_VERTEX   = 0,
    EAT_TEXCOORD    ,
    EAT_COLOR       ,
    EAT_NORMAL      ,
    EAT_SHIT            //Why not ?
};
//------------------------------------------
enum E_RENDER_TARGET_TYPE
{
	ERTT_COLOR_BUFFER_0  =  1,
    ERTT_COLOR_BUFFER_1  =  2,
    ERTT_COLOR_BUFFER_2  =  3,
    ERTT_COLOR_BUFFER_3  =  4,
    ERTT_COLOR_BUFFER_4  =  5,
	ERTT_COLOR_BUFFER_5  =  6,
	ERTT_COLOR_BUFFER_6  =  7,
	ERTT_COLOR_BUFFER_7  =  8,
	ERTT_COLOR_BUFFER_8  =  9,
	ERTT_COLOR_BUFFER_9  = 10,
	ERTT_COLOR_BUFFER_10 = 11,
	ERTT_COLOR_BUFFER_11 = 12,
	ERTT_COLOR_BUFFER_12 = 13,
	ERTT_COLOR_BUFFER_13 = 14,
	ERTT_COLOR_BUFFER_14 = 15,
	ERTT_COLOR_BUFFER_15 = 16,


    ERTT_DEPTH_BUFFER   = 64,
    ERTT_STENCIL_BUFFER = 65,

};

}
}
#endif // ERENDERERENUMS_H
