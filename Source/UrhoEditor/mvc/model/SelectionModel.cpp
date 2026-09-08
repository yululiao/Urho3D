#include "SelectionModel.h"

namespace Urho3DEditor {

void SelectionModel::SetSelectedNode(Node* node)
{
    if (selectedNode_ == node && selectType_ == "Node")
        return;

    selectedNode_ = node;
    selectedFile_.Clear();
    selectType_ = "Node";
    NotifyObservers();
}

void SelectionModel::SetSelectedFile(const String& filePath)
{
    if (selectedFile_ == filePath && selectType_ == "File")
        return;

    selectedNode_ = nullptr;
    selectedFile_ = filePath;
    selectType_ = "File";
    NotifyObservers();
}

void SelectionModel::ClearSelection()
{
    if (selectedNode_ == nullptr && selectedFile_.Empty() && selectType_.Empty())
        return;

    selectedNode_ = nullptr;
    selectedFile_.Clear();
    selectType_.Clear();
    NotifyObservers();
}

void SelectionModel::AddObserver(ISelectionObserver* observer)
{
    if (observer && !observers_.Contains(observer))
        observers_.Push(observer);
}

void SelectionModel::RemoveObserver(ISelectionObserver* observer)
{
    observers_.Remove(observer);
}

void SelectionModel::NotifyObservers()
{
    for (auto* observer : observers_)
    {
        if (observer)
            observer->OnSelectionChanged(selectedNode_, selectType_);
    }
}

} // namespace Urho3DEditor
