#include "EditorApp.h"
#include "Global.h"

using namespace Urho3DEditor;
#ifdef _MSC_VER
int WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	//�򿪿���̨
    AllocConsole();
    SetConsoleOutputCP(CP_UTF8);
    freopen("CONOUT$", "w", stdout);
	//----------
	//Global::InitGlobal();
    Context* context = new Context();
    EditorApp* app = new EditorApp(context);
	app->Run();
    delete context;
	return 0;
}

#else
int main(int argc, char* argv[])
{
	EditorApp* app = new EditorApp(Global::context);
	app->run();
	return 0;
}
#endif
