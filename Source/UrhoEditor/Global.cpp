#include "Global.h"

namespace Urho3DEditor {

Context* Global::context = nullptr;
void Global::InitGlobal(){
	context = new Context();
}

}
