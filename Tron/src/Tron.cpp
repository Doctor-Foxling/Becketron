#include <Becketron.h>
#include <Becketron/Core/EntryPoint.h>

#include "Tests.h"
#include "examples/example1.h"
#include "EditorLayer.h"

namespace Becketron
{
	class Tron : public Application
	{
	public:
		Tron()
			: Application("Tron")
		{
			//PushLayer(new EditorLayer());
			//PushLayer(new ExampleLayer1());
			PushLayer(new Tests());
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