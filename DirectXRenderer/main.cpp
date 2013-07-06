#include "Renderer.h"

using namespace Ab2;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdShow)
{
	Renderer* renderer;
	bool result;
	
	renderer = new Renderer();
	if(!renderer)
	{
		return 1;
	}

	result = renderer->Initialize();
	if(result)
	{
		renderer->Run();
	}

	renderer->Shutdown();
	delete renderer;
	renderer = NULL;

	return 0;
}