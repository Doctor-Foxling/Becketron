#pragma once

#include "Core.h"

namespace Becketron
{
	class BTRON_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	};

	// To be defined in client
	Application* CreateApplication();

}