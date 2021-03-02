#pragma once

#ifdef BT_PLATFORM_WINDOWS

extern Becketron::Application* Becketron::CreateApplication();

int main(int argc, char** argv)
{ 
	Becketron::Log::Init();

	BT_PROFILE_BEGIN_SESSION("Startup", "BtronProfile-Startup.json");
	auto app = Becketron::CreateApplication();
	BT_PROFILE_END_SESSION();

	BT_PROFILE_BEGIN_SESSION("Runtime", "BtronProfile-Runtime.json");
	app->Run();
	BT_PROFILE_END_SESSION();

	BT_PROFILE_BEGIN_SESSION("Startup", "BtronProfile-Shutdown.json");
	delete app;
	BT_PROFILE_END_SESSION();
}


#else
	#err or Becketron only supports Windows

#endif