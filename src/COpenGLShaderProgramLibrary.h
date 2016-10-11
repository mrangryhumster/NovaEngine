#pragma once

#include <map>

#include "NLogger.h"

#include "Object.h"
#include "IRenderer.h"
#include "IMaterial.h"
#include "IShaderProgram.h"

namespace novaengine
{
	namespace renderer
	{

		class COpenGLShaderProgramLibrary : public Object
		{
		public:
			COpenGLShaderProgramLibrary(IRenderer* p_OGLRenderer);
			~COpenGLShaderProgramLibrary();

			bool initShaderPrograms();
			IShaderProgram* getShaderProgram(u32 shader_features = 0);

		private:

			IRenderer* m_OGLRenderer;
			std::map<u32, IShaderProgram*> m_ProgramsCache;
		};

	}
}