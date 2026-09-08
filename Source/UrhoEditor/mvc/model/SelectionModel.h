#pragma once

#include "Urho3D/Container/Str.h"
#include "Urho3D/Container/Vector.h"
#include "Urho3D/Scene/Node.h"

namespace Urho3DEditor {

using namespace Urho3D;

class ISelectionObserver
{
public:
    virtual ~ISelectionObserver() = default;
    virtual void OnSelectionChanged(Node* selectedNode, const String& selectType) = 0;
};

class SelectionModel
{
public:
    SelectionModel() = default;
    ~SelectionModel() = default;

    void SetSelectedNode(Node* node);
    void SetSelectedFile(const String& filePath);
    void ClearSelection();

    Node* GetSelectedNode() const { return selectedNode_; }
    const String& GetSelectedFile() const { return selectedFile_; }
    const String& GetSelectType() const { return selectType_; }

    void AddObserver(ISelectionObserver* observer);
    void RemoveObserver(ISelectionObserver* observer);

private:
    void NotifyObservers();

    Node* selectedNode_ = nullptr;
    String selectedFile_;
    String selectType_;
    PODVector<ISelectionObserver*> observers_;
};

} // namespace Urho3DEditor
