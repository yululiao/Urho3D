#include "Global.h"

namespace Urho3DEditor {

Context* Global::context = nullptr;
String Global::notShowTag = "NotShowInNodeTree";
String Global::curSelectType = "";
void Global::InitGlobal(){
	context = new Context();
}

}
