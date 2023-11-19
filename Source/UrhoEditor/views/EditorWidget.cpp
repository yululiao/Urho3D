#include "EditorWidget.h"
#include "imgui/imgui_internal.h"

namespace Urho3DEditor {

bool EditorWidget::IsInWindow(ImVec2 pos) {
    ImVec2 rightBottom(_winPos.x + winSize.x, _winPos.y + winSize.y);
    ImRect winRec(_winPos, rightBottom);
    if (winRec.Contains(pos)) {
        return true;
    }
    return false;
}

}