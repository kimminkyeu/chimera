#include "Chimera.h"
#include <memory>
// #include "Chimera/Core/EntryPoint.h"
// #include "EditorLayer.h"

class Scop : public Cm::Application
{
public:
	Scop(/* ... */)
		: Application()
	{
		std::cout << "hello scope!" << std::endl;
		// PushLayer(new EditorLayer());
	}
};

std::unique_ptr<Cm::Application> Cm::CreateApplication(/* specification */)
{
	return std::make_unique<Scop>(/* specification */);
}

int main(int ac, char** av) 
{
    auto scop = Cm::CreateApplication();
	scop->Run();
	// delete scop; 
    return 0;
}
