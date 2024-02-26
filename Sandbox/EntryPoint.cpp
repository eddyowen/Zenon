#include "Core/Application.h"

int main()
{
	zn::Application app{};
	if (app.Init())
	{
		app.Run();
	}

	return 0;
}