#pragma once

#include "Becketron/Renderer/GraphicsContext.h"

// Forward declaring the GLFWwindow so we don't have to include it
struct GLFWwindow;

namespace Becketron {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}