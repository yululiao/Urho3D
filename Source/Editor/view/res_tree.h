#include <QTreeView>
#include <QFileSystemModel>

namespace urho3d
{
namespace editor
{

class res_tree :public QTreeView
{
public:
	res_tree(QWidget* p);
	~res_tree();

protected:
	virtual void showEvent(QShowEvent *event);
private:
	QFileSystemModel* _model = nullptr;

};
}
}