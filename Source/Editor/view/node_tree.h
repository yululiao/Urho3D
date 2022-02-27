#include <QTreeView>
#include "model/NodeTreeModel.h"
#include "ctrl/global_event.h"


namespace urho3d
{
namespace editor
{
class node_tree : public QTreeView
{
public:
	node_tree(QWidget* p);
	~node_tree();
public slots:
    void onCustomContextMenu(const QPoint& p);
    void onDeleteNode();

protected:
    void onCreateScene(event_data* data);
    void createContexMenus(); //ÓÒ¼ü²Ëµ¥
private:
    NodeThreeModel* _model = nullptr;
    QMenu* _contexMenu = nullptr;
    QAction* _actionDelete = nullptr;
};
}
}
