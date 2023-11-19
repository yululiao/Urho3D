#include "EditorApp.h"

using namespace Urho3DEditor;
#ifdef _MSC_VER
int WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	//打开控制台
    AllocConsole();
    SetConsoleOutputCP(CP_UTF8);
    freopen("CONOUT$", "w", stdout);
	//----------
	EditorApp::GetInstance()->Run();
	return 0;
}

#else
int main(int argc, char* argv[])
{
	EditorApp::getInstance()->run();
	return 0;
}
#endif
