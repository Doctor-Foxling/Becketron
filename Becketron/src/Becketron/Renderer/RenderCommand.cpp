#include "btpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Becketron {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}