#include "CMaterial.h"

namespace novaengine
{
namespace renderer
{

CMaterial::CMaterial():
    m_ShaderProgram(nullptr),
	m_Textures{}
{
    //ctor
}
//------------------------------------------------------------------------------
CMaterial::~CMaterial()
{
	for (auto&& Texture : m_Textures)
		if (Texture) Texture->release();

	for (auto&& Uniform : m_UniformArray)
		if (Uniform.uniform_data != nullptr) delete[] Uniform.uniform_data;

    if(m_ShaderProgram)
        m_ShaderProgram->release();
}
//------------------------------------------------------------------------------
void CMaterial::setTexture(ITexture * texture, u32 texture_unit)
{
	assert(texture_unit < ERTU_TEXTURE_COUNT);
	
	if (texture)
		texture->capture();
	if (m_Textures[texture_unit])
		m_Textures[texture_unit]->release();
	m_Textures[texture_unit] = texture;
}
//------------------------------------------------------------------------------
void CMaterial::setValue(const std::string uniform_name, const void* data, const size_t size)
{
	auto it = m_UniformNamesMap.find(uniform_name);
	if (it == m_UniformNamesMap.end())
	{
		LOG_DEBUG("Unable to set uniform to material - uniform_name not exist (uniform_name:%s)\n", uniform_name.c_str());
		return;
	}

	if (data == nullptr)
	{
		delete[](*it).second->uniform_data;
		(*it).second->uniform_data = nullptr;
	}
	else
	{
		if ((*it).second->uniform_data == nullptr)
			(*it).second->uniform_data = new u8[(*it).second->uniform_size];
		memcpy((*it).second->uniform_data, data, (size == 0) ? (*it).second->uniform_size : size);
	}
}
//------------------------------------------------------------------------------
void CMaterial::setValue(const s32 uniform_location, const void * data, const size_t size)
{
	auto it = m_UniformLocationsMap.find(uniform_location);
	if (it == m_UniformLocationsMap.end())
	{
		LOG_DEBUG("Unable to set uniform to material - uniform_location not exist (uniform_location:%s)\n", uniform_location);
		return;
	}

	if (data == nullptr)
	{
		delete[](*it).second->uniform_data;
		(*it).second->uniform_data = nullptr;
	}
	else
	{
		if ((*it).second->uniform_data == nullptr)
			(*it).second->uniform_data = new u8[(*it).second->uniform_size];
		memcpy((*it).second->uniform_data, data, (size == 0) ? (*it).second->uniform_size : size);
	}
}
//------------------------------------------------------------------------------
void CMaterial::setShaderProgram(IShaderProgram* newProgram)
{
// NOTE (mrang#1#): Maybe "if new!=old" is useless here

	if (newProgram != nullptr)
		newProgram->capture();
    if(m_ShaderProgram)
        m_ShaderProgram->release();

    m_ShaderProgram = newProgram;
}
//------------------------------------------------------------------------------
IShaderProgram* CMaterial::getShaderProgram()
{
    return m_ShaderProgram;
}
//------------------------------------------------------------------------------
void CMaterial::build()
{
	//clear previous info and delete allocated cache
	for (auto&& Uniform : m_UniformArray)
		if (Uniform.uniform_data != nullptr) delete[] Uniform.uniform_data;

	m_UniformLocationsMap.clear();
	m_UniformNamesMap.clear();
	m_UniformArray.clear();

	m_Uniform_ne_texture1d_location = -1;
	m_Uniform_ne_texture1d_count    =  0;
	m_Uniform_ne_texture2d_location = -1;
	m_Uniform_ne_texture2d_count	=  0;
	m_Uniform_ne_texture3d_location = -1;
	m_Uniform_ne_texture3d_count	=  0;

	//exit if no associated shader program
	if (m_ShaderProgram == nullptr)
		return;

	//load uniform info
	u32 ShaderUniformInfoCount = m_ShaderProgram->getUniformInfoCount();
	const SShaderUniformInfo* ShaderUniformInfo = m_ShaderProgram->getUniformsInfo();

	m_UniformArray.reserve(ShaderUniformInfoCount);
	for (u32 i = 0; i < ShaderUniformInfoCount; i++)
	{
		//--------------------
		if (ShaderUniformInfo[i].name == "ne_texture1d" && ShaderUniformInfo[i].type == ESUT_TEXTURE_1D)
		{
			m_Uniform_ne_texture1d_location = ShaderUniformInfo[i].location;
			m_Uniform_ne_texture1d_count    = ShaderUniformInfo[i].count;
		}
		else if (ShaderUniformInfo[i].name == "ne_texture2d" && ShaderUniformInfo[i].type == ESUT_TEXTURE_2D)
		{
			m_Uniform_ne_texture2d_location = ShaderUniformInfo[i].location;
			m_Uniform_ne_texture2d_count    = ShaderUniformInfo[i].count;
		}
		else if (ShaderUniformInfo[i].name == "ne_texture3d" && ShaderUniformInfo[i].type == ESUT_TEXTURE_3D)
		{
			m_Uniform_ne_texture3d_location = ShaderUniformInfo[i].location;
			m_Uniform_ne_texture3d_count    = ShaderUniformInfo[i].count;
		}
		//--------------------
		m_UniformArray.push_back(SMaterialUniformCache(ShaderUniformInfo[i]));
		SMaterialUniformCache* Ref = &m_UniformArray.back();

		m_UniformLocationsMap.insert(std::unordered_map<s32, SMaterialUniformCache*>::value_type(ShaderUniformInfo[i].location, Ref));
		m_UniformNamesMap.insert(std::unordered_map<std::string, SMaterialUniformCache*>::value_type(ShaderUniformInfo[i].name, Ref));
	}
	LOG_ENGINE_DEBUG("Loaded info about %d uniforms\n", ShaderUniformInfoCount);
}
//------------------------------------------------------------------------------
void CMaterial::bind()
{
    m_ShaderProgram->bind();
	//first default fields...

	//--------------------
	//textures
	s32 texture_units_1d[ERTU_TEXTURE_COUNT];
	u32 texture_units_1d_inc = 0;
	s32 texture_units_2d[ERTU_TEXTURE_COUNT];
	u32 texture_units_2d_inc = 0;
	s32 texture_units_3d[ERTU_TEXTURE_COUNT];
	u32 texture_units_3d_inc = 0;

	u32 texture_index = 0;
	for (auto&& Texture : m_Textures)
	{
		if (Texture)
		{
			getRenderer()->bindTexture(Texture, texture_index);
			//check texture type
			//TODO FIXME: insert texture type check

			switch (1)
			{
			case 0: texture_units_1d[texture_units_1d_inc++] = texture_index; break;
			case 1: texture_units_2d[texture_units_2d_inc++] = texture_index; break;
			case 2: texture_units_3d[texture_units_3d_inc++] = texture_index; break;
			}

		}
		texture_index++;
	}

	if (m_Uniform_ne_texture1d_location != -1)
		m_ShaderProgram->setUniform(m_Uniform_ne_texture1d_location, ESUT_TEXTURE_1D, m_Uniform_ne_texture1d_count, texture_units_1d);
	if (m_Uniform_ne_texture2d_location != -1)
		m_ShaderProgram->setUniform(m_Uniform_ne_texture2d_location, ESUT_TEXTURE_2D, m_Uniform_ne_texture2d_count, texture_units_2d);
	if (m_Uniform_ne_texture3d_location != -1)
		m_ShaderProgram->setUniform(m_Uniform_ne_texture3d_location, ESUT_TEXTURE_3D, m_Uniform_ne_texture3d_count, texture_units_3d);

	//--------------------
	
	//second custom fields
	for (auto&& Uniform : m_UniformArray)
	{
		if (Uniform.uniform_data != nullptr)
			m_ShaderProgram->setUniform(Uniform.uniform_location, Uniform.uniform_type, Uniform.uniform_count, Uniform.uniform_data);
	}
}
//------------------------------------------------------------------------------

}
}
