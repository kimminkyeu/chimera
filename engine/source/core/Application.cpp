// #include "hzpch.h"

#include <iostream>

#include "internal/CmUtil.h"
#include "core/Application.h"

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