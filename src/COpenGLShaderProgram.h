#ifndef COPENGLSHADERPROGRAM_H
#define COPENGLSHADERPROGRAM_H

#include <unordered_map>
#include <vector>

#include "IRenderer.h"
#include "IShaderProgram.h"
#include "NLogger.h"
#include <GL/glew.h>

namespace novaengine
{
namespace renderer
{

class COpenGLShaderProgram : public IShaderProgram
{
public:
    COpenGLShaderProgram(IRenderer* p_OGLRenderer);
    virtual ~COpenGLShaderProgram();

    void setVertexShaderSource(const char*);
    void setFragmentShaderSource(const char*);

    bool build();

	u32                       getUniformInfoCount();
	const SShaderUniformInfo* getUniformsInfo();

    s32 getUniformLocation(const char* UniformName);

	void setUniform(s32 uniform_location, u32 uniform_type, u32 uniform_count, const void* data);
	void setUniform(const char* uniform_name, u32 uniform_type, u32 uniform_count, const void* data);

    void bind();

    u32 getLastError();
	//OpenGLShaderProgram public:
    u32 getProgramID();
protected:
private:
	void update_uniform_info();
    void shader_log(GLuint source);


    IRenderer* m_OGLRenderer;

    // NOTE (mrang#1#): Maybe map with const char* ?
	std::unordered_map<std::string, GLint> m_UniformNames;
	std::vector<SShaderUniformInfo>        m_Uniforms;


	//OpenGL
    GLuint m_GLVertexShader;
    GLuint m_GLFragmentShader;
    GLuint m_GLProgram;
	//depricated
    u32 m_LastError;

};

}
}

#endif // COPENGLSHADERPROGRAM_H
