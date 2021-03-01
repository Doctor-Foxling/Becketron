#include "btpch.h"
#include "RenderCommand.h"

namespace Becketron {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}