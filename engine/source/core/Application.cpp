#include "core/Pch.h"

#include "internal/CmUtil.h"
#include "core/Application.h"

namespace Cm {

Application::Application() {}

Application::~Application() { std::cout << "shut down" << std::endl; }

void Application::Run() {
  CM_CORE_INFO("Run Start");
  while (42) {
  }
}
} // namespace Cm