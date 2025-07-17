#include "NdfMgr.h"
#include "Global.h"
#include "view/ndf/nfd.h"
#include "ctrl/res/AssetMgr.h"

namespace Urho3DEditor
{
NdfMgr* NdfMgr::_instance = nullptr;
NdfMgr* NdfMgr::GetInstance()
{
	if(!_instance)
	{
		_instance = new NdfMgr(Global::context);
	}
	return _instance;
}
NdfMgr::NdfMgr(Context* context)
	:Object(context)
{

}
NdfMgr::~NdfMgr() 
{
}
void NdfMgr::GoToPath(const String& path)
{
	String pathTmp = AssetMgr::getInstance()->pathToFull(path);
#ifdef WIN32
	pathTmp.Replace("/", "\\", false);
	String pathCmd = "/select, \"";
	pathCmd = pathCmd + pathTmp + "\"";
	ShellExecute(NULL, "open", "explorer.exe", pathCmd.CString(), NULL, SW_SHOWNORMAL);
#endif // WIN32
}
}

