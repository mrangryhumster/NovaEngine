#ifndef ERENDERERENUMS_H
#define ERENDERERENUMS_H


namespace novaengine
{
namespace renderer
{

enum E_RENDERER_TYPE
{
    ERT_NULL        = 0x0,
    ERT_OPENGL      = 0x1,
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
//depricated no more fixed shit
enum E_RENDER_PIPELINE_STATE
{
    ERPS_SOME = 0
};
//------------------------------------------
enum E_RENDER_STATE
{
    ERS_ENABLE_DEPTH_TEST       = 0,
    ERS_ENABLE_DEPTH_WRITE         ,
    ERS_DEPTH_TEST_MODE            ,
	ERS_ENABLE_COLOR_WRITE         ,
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
    ERS_FOG_COLOR                  ,
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
    EMT_MVP        = 0x0,
    EMT_PROJECTION = 0x1,
    EMT_VIEW       = 0x2,
    EMT_MODEL      = 0x3,
};
//------------------------------------------
enum E_ATTRIB_TYPE
{
    EAT_VERTEX   = 0,
    EAT_TEXCOORD    ,
    EAT_COLOR       ,
    EAT_NORMAL      ,
};
//------------------------------------------
enum E_RENDER_TEXTURE_UNIT
{
	ERTU_TEXTURE_0 = 0,
	ERTU_TEXTURE_1 = 1,
	ERTU_TEXTURE_2 = 2,
	ERTU_TEXTURE_3 = 3,
	ERTU_TEXTURE_4 = 4,
	ERTU_TEXTURE_5 = 5,
	ERTU_TEXTURE_6 = 6,
	ERTU_TEXTURE_7 = 7,
	ERTU_TEXTURE_8 = 8,
	ERTU_TEXTURE_9 = 9,
	ERTU_TEXTURE_10 = 10,
	ERTU_TEXTURE_11 = 11,
	ERTU_TEXTURE_12 = 12,
	ERTU_TEXTURE_13 = 13,
	ERTU_TEXTURE_14 = 14,
	ERTU_TEXTURE_15 = 15,
	ERTU_TEXTURE_LAST = ERTU_TEXTURE_15,

	ERTU_TEXTURE_COUNT
};
//------------------------------------------
enum E_RENDER_MATERIAL_FEATURE
{
	ERMF_DEFAULT = 0x0,
	ERMF_COLOR   = 0x1,
	ERMF_TEXTURE = 0x2,
	ERMF_SOMESHIT= 0x4,
};
//------------------------------------------
enum E_RENDER_TARGET_TYPE
{

	ERTT_COLOR_BUFFER_0		 =  0,
    ERTT_COLOR_BUFFER_1		 ,
    ERTT_COLOR_BUFFER_2		 ,
    ERTT_COLOR_BUFFER_3		 ,
    ERTT_COLOR_BUFFER_4		 ,
	ERTT_COLOR_BUFFER_5		 ,
	ERTT_COLOR_BUFFER_6		 ,
	ERTT_COLOR_BUFFER_7		 ,
	ERTT_COLOR_BUFFER_8		 ,
	ERTT_COLOR_BUFFER_9		 ,
	ERTT_COLOR_BUFFER_10	 ,
	ERTT_COLOR_BUFFER_11	 ,
	ERTT_COLOR_BUFFER_12	 ,
	ERTT_COLOR_BUFFER_13	 ,
	ERTT_COLOR_BUFFER_14     ,
	ERTT_COLOR_BUFFER_15     ,
	ERTT_COLOR_BUFFER_LAST = ERTT_COLOR_BUFFER_15,

    ERTT_DEPTH_BUFFER        ,
    ERTT_STENCIL_BUFFER      ,

	ERTT_TARGET_COUNT        ,
};

}
}
#endif // ERENDERERENUMS_H
