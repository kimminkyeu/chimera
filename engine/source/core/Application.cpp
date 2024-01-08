// #include "hzpch.h"

#include "CmUtil.h"
#include "core/Application.h"
#include <iostream>

// #include "Chimera/Core/Log.h"

// #include "Chimera/Renderer/Renderer.h"
// #include "Chimera/Scripting/ScriptEngine.h"

// #include "Chimera/Core/Input.h"
// #include "Chimera/Utils/PlatformUtils.h"

namespace Cm {


	Application::Application()
	{
	}

	Application::~Application()
	{
		std::cout << "shut down" << std::endl;
	}

	void Application::Run()
	{
		std::cout << "run" << std::endl;
		while(42)
		{
			
		}
	}
}