#include <Becketron.h>
#include <Becketron/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Becketron
{
	class Tron : public Application
	{
	public:
		Tron()
			: Application("Tron")
		{
			PushLayer(new EditorLayer());
		}

		~Tron()
		{
		}
	};

	Application* CreateApplication()
	{
		return new Tron();
	}
}