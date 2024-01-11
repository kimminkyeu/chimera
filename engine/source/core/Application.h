#pragma once
#ifndef CHIMERA_ENGINE_SOURCE_CORE_APPLICATION_H
#define CHIMERA_ENGINE_SOURCE_CORE_APPLICATION_H

#include <memory>

#ifdef _CHIMERA_BUILD_DLL
	#define CHIMERA_API __declspec(dllexport)
#else
	#define CHIMERA_API __declspec(dllimport)
#endif

namespace Cm {

	class CHIMERA_API Application
	{
	public:
		Application();
		virtual ~Application();

	public:
		void Run();
	};

	// To be defined in CLIENT
	std::unique_ptr<Application> CreateApplication();
}

#endif //CHIMERA_ENGINE_SOURCE_CORE_APPLICATION_H