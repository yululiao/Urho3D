#include "set"
#include <QTreeView>
#include <QFileSystemModel>
#include "model/res_model.h"


namespace urho3d
{
namespace editor
{

class res_tree :public QTreeView
{
public:
	res_tree(QWidget* p);
	~res_tree();
public slots:
    void OnCustomContextMenu(const QPoint &p);
    void OnItemDoubleClicked(const QModelIndex i);
    void OnItemClicked(const QModelIndex i);
    void OnCreateMaterial();
    void OnAddDir();
    void OnShowInExplorer();
    void OnRemoveItem();
    void OnImportAssets();
protected:
	virtual void showEvent(QShowEvent *event);
    void createContexMenus();//资源树右键菜单
private:
    res_model* _model = nullptr;
    /*contexmenu*/
    QMenu* contexMenu = nullptr;
    QAction* actionCreateFile = nullptr;
    QMenu* menuCreateFile = nullptr;
    QAction* actionCreateMat = nullptr;
    QAction* actionCreateActions = nullptr;
    QAction* actionCreateDir = nullptr;

    QAction* actionImportAssets = nullptr;
    QAction* actionShowinExpl = nullptr;
    QAction* actionDeletefile = nullptr;
    std::set<QString>  espandSet;

    //QModelIndex lastIndex;
    QString lastNodeName;
    QString lastAboveName;
    QString workSpace;
    QString assetRoot;

};
}
}
