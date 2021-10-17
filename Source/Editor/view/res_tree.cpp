#include "res_tree.h"
#include "ctrl/work_space.h"
#include "ctrl/asset_mgr.h"
#include "ctrl/utils.h"


namespace urho3d
{
namespace editor
{
res_tree::res_tree(QWidget* p)
	:QTreeView(p)
{
	
	_model = new QFileSystemModel(p);
    std::string root = asset_mgr::get_instance()->get_asset_path();
	_model->setRootPath(root.c_str());
	_model->setReadOnly(false);
	this->setModel(_model);
	this->setRootIndex(_model->index(root.c_str()));
	this->setHeaderHidden(true);
	this->hideColumn(1);
	this->hideColumn(2);
	this->hideColumn(3);
	
}
res_tree::~res_tree()
{
	

}

void res_tree::showEvent(QShowEvent *event)
{
	QTreeView::showEvent(event);
	
}
}
}
