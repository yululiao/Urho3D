#include <QMenu>
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
    createContexMenus();
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    global_event::get_instance()->register_event(eGlobalEventType::CreateScene,MakeDelegate(this,&node_tree::onCreateScene));
    connect(this, &QTreeView::customContextMenuRequested, this, &node_tree::onCustomContextMenu);
}
node_tree::~node_tree()
{

}

void node_tree::createContexMenus()
{ 
    _contexMenu = new QMenu();
    _actionDelete = _contexMenu->addAction("delete");
    connect(_actionDelete,&QAction::triggered,this,&node_tree::onDeleteNode);
}

void node_tree::onCustomContextMenu(const QPoint& p)
{
    _actionDelete->setEnabled(true);
    QModelIndexList indexList = this->selectedIndexes();
    if (indexList.size() == 0)
    {
        _actionDelete->setEnabled(false);
    }
    _contexMenu->exec(QCursor::pos());
}

void node_tree::onDeleteNode()
{ 
    QModelIndexList indexList = this->selectedIndexes();
    for (auto& item:indexList)
    {
        _model->remove(item);
    }
}

void node_tree::onCreateScene(event_data* data)
{
    Node* root = SceneCtrl::get_inatance()->getRoot();

    _model->SetRootNode(root);
}

}
}
