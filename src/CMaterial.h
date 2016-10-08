#ifndef CMATERIAL_H
#define CMATERIAL_H

#include <assert.h>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>

#include "NovaEngine.h"
#include "ne_types.h"

#include "IRenderer.h"
#include "IMaterial.h"

#include "NLogger.h"
#include "GL/glew.h"

namespace novaengine
{
namespace renderer
{

struct SMaterialUniformCache
{
	SMaterialUniformCache(const SShaderUniformInfo& UniformInfo) :
		uniform_location(UniformInfo.location),uniform_type(UniformInfo.type),uniform_count(UniformInfo.count), uniform_size(UniformInfo.size), uniform_data(nullptr)
	{}

	~SMaterialUniformCache()
	{}

	s32  uniform_location;

	u32	 uniform_type;
	u32	 uniform_count;
	u32  uniform_size;

	u8*  uniform_data;
};


class CMaterial : public IMaterial
{
    public:
        CMaterial();
        virtual ~CMaterial();

		void setTexture(ITexture* texture, u32 texture_unit);

		void setValue(const std::string uniform_name,const void* data, const size_t size = 0);
		void setValue(const s32 uniform_location,const void* data, const size_t size = 0);

        void            setShaderProgram(IShaderProgram*);
        IShaderProgram* getShaderProgram();

		void build();
        void bind();

    protected:
    private:

		IShaderProgram* m_ShaderProgram;
		

		ITexture* m_Textures[ERTU_TEXTURE_COUNT];

		std::unordered_map<std::string, SMaterialUniformCache*> m_UniformNamesMap;
		std::unordered_map<s32        , SMaterialUniformCache*> m_UniformLocationsMap;
		std::vector<SMaterialUniformCache> m_UniformArray;

		//engine default uniform location
		//simple
		s32 m_Uniform_location_ne_ambient_color;	// ne_ambient_color
		s32 m_Uniform_location_ne_diffuse_color;	// ne_diffuse_color
		s32 m_Uniform_location_ne_specular_color;   // ne_specular_color
		//textures
		s32 m_Uniform_ne_texture1d_location; //ne_texture1d
		u32 m_Uniform_ne_texture1d_count;
		s32 m_Uniform_ne_texture2d_location; //ne_texture2d
		u32 m_Uniform_ne_texture2d_count;
		s32 m_Uniform_ne_texture3d_location; //ne_texture3d
		u32 m_Uniform_ne_texture3d_count;
};

}
}

#endif // CMATERIAL_H
