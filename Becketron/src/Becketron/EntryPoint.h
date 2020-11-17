#pragma once

#ifdef BT_PLATFORM_WINDOWS

extern Becketron::Application* Becketron::CreateApplication();

int main(int argc, char** argv)
{
	printf("Becky Engine");
	auto app = Becketron::CreateApplication();
	app->Run();
	delete app;
}


#else
	#error Becketron only supports Windows

#endif