#include "node_tree.h"
#include "ctrl/scene_ctrl.h"

namespace urho3d
{
namespace editor
{
node_tree::node_tree(QWidget* p)
	:QTreeView(p)
{
    _model = new NodeThreeModel(p);

    this->setModel(_model);
    global_event::get_instance()->register_event(eGlobalEventType::CreateScene,MakeDelegate(this,&node_tree::onCreateScene));
}
node_tree::~node_tree()
{

}

void node_tree::onCreateScene(event_data* data)
{
    Node* root = scene_ctrl::get_inatance()->getRoot();

    _model->SetRootNode(root);
}

}
}
