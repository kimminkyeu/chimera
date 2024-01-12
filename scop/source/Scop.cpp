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
  CM_CLIENT_INFO("Scop Start");
  auto scop = Cm::CreateApplication();
  scop->Run();
  return 0;
}
