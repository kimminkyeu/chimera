#pragma once
#ifndef CHIMERA_ENGINE_SOURCE_CORE_APPLICATION_H
#define CHIMERA_ENGINE_SOURCE_CORE_APPLICATION_H

// #include "internal/CmUtil.h"
#include <memory>

#ifdef _CHIMERA_BUILD_DLL
	#define CHIMERA_API __declspec(dllexport)
#else
	#define CHIMERA_API __declspec(dllimport)
#endif
// #include "Chimera/Core/Base.h"

// #include "Chimera/Core/Window.h"
// #include "Chimera/Core/LayerStack.h"
// #include "Chimera/Events/Event.h"
// #include "Chimera/Events/ApplicationEvent.h"

// #include "Chimera/Core/Timestep.h"

// #include "Chimera/ImGui/ImGuiLayer.h"

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