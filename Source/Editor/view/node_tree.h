#include <QTreeView>


namespace urho3d
{
namespace editor
{
class node_tree : public QTreeView
{
public:
	node_tree(QWidget* p);
	~node_tree();

};
}
}