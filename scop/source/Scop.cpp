#include "Chimera.h"

class Scop : public Cm::Application {
public:
  Scop() : Application() { std::cout << "hello scope!" << std::endl; }
};

std::unique_ptr<Cm::Application> Cm::CreateApplication() {
  return std::make_unique<Scop>();
}

int main(int ac, char** av) {
  Cm::Logger::Init();
  auto scop = Cm::CreateApplication();
  CM_LOG(INFO, "Welcome to spdlog!");
  CM_LOG(ERROR, "Some error message with arg: {}", 1);
  
  CM_LOG(WARN, "Easy padding in numbers like {:08d}", 12);
  CM_LOG(FATAL, "Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
  CM_LOG(INFO, "Support for floats {:03.2f}", 1.23456);
  CM_LOG(INFO, "Positional args are {1} {0}..", "too", "supported");
  CM_LOG(INFO, "{:<30}", "left aligned");
  
  spdlog::set_level(spdlog::level::debug); // Set global log level to debug
  spdlog::debug("This message should be displayed..");    
    
  scop->Run();
  
  return 0;
}
