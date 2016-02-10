#ifdef NE_OPENGLES1_RENDERER


#include "COpenGLES1VertexBuffer.h"

namespace novaengine
{
namespace renderer
{
COpenGLES1VertexBuffer::COpenGLES1VertexBuffer():
    OpenGL_VBO_verticles_ID(0),
    OpenGL_VBO_indices_ID(0),
    OpenGL_VAO_ID(0),
    OpenGL_VBO_Status(EVBT_UNDEFINDED),
    OpenGL_VBO_Format(0),
    OpenGL_VBO_VerticesCount(0)
{

}

COpenGLES1VertexBuffer::~COpenGLES1VertexBuffer()
{

}

u32 COpenGLES1VertexBuffer::getVertexCount()
{
    /*
    if(OpenGL_VBO_Status == EVBT_VBO_STREAM or OpenGL_VBO_Status == EVBT_VBO_STATIC)
        return OpenGL_VBO_VerticesCount;
    else
    */
    return CVertexBuffer::getVertexCount();
}

void COpenGLES1VertexBuffer::lock()
{
/*
    if(OpenGL_VBO_Status == EVBT_VBO_STREAM or OpenGL_VBO_Status == EVBT_VBO_STATIC)
    {
        glBindBuffer(GL_ARRAY_BUFFER,OpenGL_VBO_verticles_ID);
        if(OpenGL_VBO_indices_ID)
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,OpenGL_VBO_indices_ID);

        int VertexCount = OpenGL_VBO_VerticesCount;

        u32 BufferShift = 0;
        if(OpenGL_VBO_Format & EVF_VERTEX)
        {
            //glBufferSubData(GL_ARRAY_BUFFER,BufferShift,VertexCount * sizeof(core::vector3f),getPositions());
            core::vector3f* restored_Positions = new core::vector3f[VertexCount];
            glGetBufferSubData(GL_ARRAY_BUFFER,BufferShift,VertexCount * sizeof(core::vector3f),restored_Positions);
            setPositions(restored_Positions,VertexCount);
            BufferShift+=VertexCount * sizeof(core::vector3f);

            delete[] restored_Positions;
        }
        if(OpenGL_VBO_Format & EVF_TEXCOORD)
        {
            core::vector2f* restored_UVs = new core::vector2f[VertexCount];
            glGetBufferSubData(GL_ARRAY_BUFFER,BufferShift,VertexCount * sizeof(core::vector2f),restored_UVs);
            setUVs(restored_UVs,VertexCount);
            BufferShift+=VertexCount * sizeof(core::vector2f);

            delete[] restored_UVs;
        }
        if(OpenGL_VBO_Format & EVF_NORMAL)
        {
            core::vector3f* restored_Normals = new core::vector3f[VertexCount];
            glGetBufferSubData(GL_ARRAY_BUFFER,BufferShift,VertexCount * sizeof(core::vector3f),restored_Normals);
            setNormals(restored_Normals,VertexCount);
            BufferShift+=VertexCount * sizeof(core::vector3f);

            delete[] restored_Normals;
        }
        if(OpenGL_VBO_Format & EVF_VERTEX_COLOR)
        {
            core::color4u* restored_Colors = new core::color4u[VertexCount];
            glGetBufferSubData(GL_ARRAY_BUFFER,BufferShift,VertexCount * sizeof(core::vector3f),restored_Colors);
            setColors(restored_Colors,VertexCount);

            BufferShift+=VertexCount * sizeof(core::color4u);

            delete[] restored_Colors;
        }

        VertexFormat = OpenGL_VBO_Format;

        if(OpenGL_VBO_indices_ID)
        {
            int BufferSize = 0;
            glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &BufferSize);
            u32* restored_indices = new u32[BufferSize / sizeof(u32)];
            glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,BufferSize,restored_indices);
            Indices.clear();
            setIndices(restored_indices,BufferSize / sizeof(u32));
            delete[] restored_indices;
        }

        glBindBuffer(GL_ARRAY_BUFFER,0);
        if(OpenGL_VBO_indices_ID)
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    }
    */

}

void COpenGLES1VertexBuffer::unlock()
{
    Update();
}

void COpenGLES1VertexBuffer::bind_buffer()
{
    /*
    //!< bind VAO, if VAO not supported we bind VBO vertex and VBO indices
    if(GLEW_ARB_vertex_array_object)
    {
        glBindVertexArray(OpenGL_VAO_ID);
    }
    else if(GLEW_ARB_vertex_buffer_object)
    {
        glBindBuffer(GL_ARRAY_BUFFER,OpenGL_VBO_verticles_ID);
        if(OpenGL_VBO_indices_ID)
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,OpenGL_VBO_indices_ID);
    }
    */
}
void COpenGLES1VertexBuffer::unbind_buffer()
{
    /*
    //!< unbind VAO(or VBO)
    if(GLEW_ARB_vertex_array_object)
    {
        glBindVertexArray(0);
    }
    if(GLEW_ARB_vertex_buffer_object)
    {
        glBindBuffer(GL_ARRAY_BUFFER,0);
        if(OpenGL_VBO_indices_ID)
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    }
    */
}
bool COpenGLES1VertexBuffer::Update()
{
    /*
    if(OpenGL_VBO_Status == BufferType || (OpenGL_VBO_Status == EVBT_UNDEFINDED && BufferType == EVBT_RAWDATA))
    {
        if(OpenGL_VBO_Status == EVBT_VBO_STREAM)
        {
            build_vbo();
            if(GLEW_ARB_vertex_buffer_object)
                update_vao();
        }
        return false;
    }
    if( (OpenGL_VBO_Status == EVBT_RAWDATA or OpenGL_VBO_Status == EVBT_UNDEFINDED) and
            (BufferType == EVBT_VBO_STREAM or BufferType == EVBT_VBO_STATIC))
    {
        if(not is_vbo_suported())
        {
            BufferType = EVBT_RAWDATA;
            return false;
        }

        init_vbo();
        OpenGL_VBO_Status = BufferType;
        OpenGL_VBO_Format = VertexFormat;
    }

    if( (OpenGL_VBO_Status == EVBT_VBO_STREAM or OpenGL_VBO_Status == EVBT_VBO_STATIC) and
            (BufferType == EVBT_RAWDATA))
    {
        deinit_vbo();
        OpenGL_VBO_Status = BufferType;
    }
    */
    UpdateRequired = false;
    return true;
}
void COpenGLES1VertexBuffer::render()
{
    getRenderer()->drawVertexBuffer(this);
}
bool COpenGLES1VertexBuffer::is_vbo_suported()
{
    /*
    if(GLEW_ARB_vertex_array_object or GLEW_ARB_vertex_buffer_object)
        return true;
    else
    */
        return false;
}
void COpenGLES1VertexBuffer::init_vbo()
{
    /*
    u32 IndexCount    = Indices.size();

    //!< Preparing VBO
    if(GLEW_ARB_vertex_buffer_object)
    {
        //!< generate, bind VBO and upload vertex data to them
        glGenBuffers ( 1, &OpenGL_VBO_verticles_ID );

        if(IndexCount)
            glGenBuffers ( 1, &OpenGL_VBO_indices_ID );

        build_vbo();

        //!< Unbind buffers
        glBindBuffer(GL_ARRAY_BUFFER,0);
        if(IndexCount)
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    }

    //!< Preparing VAO(if supported)
    if(GLEW_ARB_vertex_array_object)
    {
        //!< generate and bind VAO id
        glGenVertexArrays(1,&OpenGL_VAO_ID);

        update_vao();
    }

    if(BufferType == EVBT_VBO_STATIC)
    {
//        Vertices.clear();
        if(Indices.size())
            Indices.clear();
        UpdateRequired = false;
    }
    */
}
void COpenGLES1VertexBuffer::deinit_vbo()
{
    /*
    if(OpenGL_VAO_ID)
    {
        glDeleteVertexArrays(1,&OpenGL_VAO_ID);
    }
    if(GLEW_ARB_vertex_buffer_object)
    {
        glDeleteBuffers(1,&OpenGL_VBO_verticles_ID);
        if(!OpenGL_VBO_indices_ID)
            glDeleteBuffers(1,&OpenGL_VBO_indices_ID);
    }
    */
}
void COpenGLES1VertexBuffer::build_vbo()
{
    /*
    u32 IndexCount    = Indices.size();
    u32 VertexCount   = Positions.size();

    if(VertexCount == 0)
        return;

    OpenGL_VBO_VerticesCount = VertexCount;

    u32 BufferSize = 0;
    if(VertexFormat & EVF_VERTEX)
        BufferSize += VertexCount * sizeof(core::vector3f);
    if(VertexFormat & EVF_TEXCOORD)
        BufferSize += VertexCount * sizeof(core::vector2f);
    if(VertexFormat & EVF_NORMAL)
        BufferSize += VertexCount * sizeof(core::vector3f);
    if(VertexFormat & EVF_VERTEX_COLOR)
        BufferSize += VertexCount * sizeof(core::color4u);

    glBindBuffer(GL_ARRAY_BUFFER,OpenGL_VBO_verticles_ID);

    glBufferData ( GL_ARRAY_BUFFER, BufferSize, 0, (BufferType == EVBT_VBO_STREAM) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW );

    u32 BufferShift = 0;
    if(VertexFormat & EVF_VERTEX)
    {
        glBufferSubData(GL_ARRAY_BUFFER,BufferShift,VertexCount * sizeof(core::vector3f),getPositions());
        BufferShift+=VertexCount * sizeof(core::vector3f);
    }
    if(VertexFormat & EVF_TEXCOORD)
    {
        glBufferSubData(GL_ARRAY_BUFFER,BufferShift,VertexCount * sizeof(core::vector2f),getUVs());
        BufferShift+=VertexCount * sizeof(core::vector2f);
    }
    if(VertexFormat & EVF_NORMAL)
    {
        glBufferSubData(GL_ARRAY_BUFFER,BufferShift,VertexCount * sizeof(core::vector3f),getNormals());
        BufferShift+=VertexCount * sizeof(core::vector3f);
    }
    if(VertexFormat & EVF_VERTEX_COLOR)
    {
        glBufferSubData(GL_ARRAY_BUFFER,BufferShift,VertexCount * sizeof(core::color4u),getColors());
        BufferShift+=VertexCount * sizeof(core::color4u);
    }
    glBindBuffer(GL_ARRAY_BUFFER,0);

    if(IndexCount)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,OpenGL_VBO_indices_ID);
        glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * IndexCount, Indices.data(), (BufferType == EVBT_VBO_STREAM) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW );
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    }
*/
}
void COpenGLES1VertexBuffer::update_vao()
{
    /*
    u32 IndexCount    = Indices.size();
    u32 VertexCount   = Positions.size();

    if(VertexCount == 0)
        return;

    glBindVertexArray(OpenGL_VAO_ID);

    //!< bind VBO vertex id
    glBindBuffer ( GL_ARRAY_BUFFER, OpenGL_VBO_verticles_ID );

    //!< bind VBO indices id(if it need)
    if(IndexCount)
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, OpenGL_VBO_indices_ID);


    bool have_verticles = (VertexFormat & EVF_VERTEX);
    bool have_texcoords = (VertexFormat & EVF_TEXCOORD);
    bool have_normals   = (VertexFormat & EVF_NORMAL);
    bool have_colors    = (VertexFormat & EVF_VERTEX_COLOR);

    //!< setup VAO
    //------------------------------------------------------------
    if(have_verticles)
        glEnableClientState(GL_VERTEX_ARRAY         );
    if(have_texcoords)
        glEnableClientState(GL_TEXTURE_COORD_ARRAY  );
    if(have_normals)
        glEnableClientState(GL_NORMAL_ARRAY         );
    if(have_colors)
        glEnableClientState(GL_COLOR_ARRAY          );
    //------------------------------------------------------------
    u32 BufferShift = 0;
    if(have_verticles)
    {
        glVertexPointer     (3  ,   GL_FLOAT        ,   0 ,   (void*)  0  );
        BufferShift+=VertexCount * sizeof(core::vector3f);
    }
    if(have_texcoords)
    {
        glTexCoordPointer   (2  ,   GL_FLOAT        ,   0 ,   (void*) BufferShift  );
        BufferShift+=VertexCount * sizeof(core::vector2f);
    }
    if(have_normals)
    {
        glNormalPointer     (       GL_FLOAT        ,   0 ,   (void*) BufferShift  );
        BufferShift+=VertexCount * sizeof(core::vector3f);
    }
    if(have_colors)
    {
        glColorPointer      (4  ,   GL_UNSIGNED_BYTE,   0 ,   (void*) BufferShift  );
        BufferShift+=VertexCount * sizeof(core::color4u);
    }
    //------------------------------------------------------------

    //!< Unbind VAO
    glBindVertexArray(0);
    */
}
u32 COpenGLES1VertexBuffer::getVBO_VERTICLES_ID()
{
    return OpenGL_VBO_verticles_ID;
}
u32 COpenGLES1VertexBuffer::getVBO_INDICES_ID()
{
    return OpenGL_VBO_indices_ID;
}
u32 COpenGLES1VertexBuffer::getVAO_ID()
{
    return OpenGL_VAO_ID;
}
}
}
#endif // NE_OPENGLES1_RENDERER
