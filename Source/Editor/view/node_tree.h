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
protected:
    void onCreateScene(event_data* data);
private:
    NodeThreeModel* _model = nullptr;

};
}
}
