#include <QMenu>
#include <QAction>
#include <QDesktopServices>
#include <QProcess>
#include <QUrl>
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
	
    _model = new res_model(p);
    assetRoot = asset_mgr::get_instance()->get_asset_path().c_str();
    workSpace = work_space::get_instance()->get_workspace().c_str();
    _model->setRootPath(assetRoot);
	_model->setReadOnly(false);
    QStringList nameFilter;
    nameFilter << "*.fbx"<<"*.png"<<"*.tga"<<"*.jpg"<<"*.tif"<<"*.txt"<<"*.json"<<"*.prefab"<<".scene";
    _model->setNameFilterDisables(false);
   _model->setNameFilters(nameFilter);
    createContexMenus();
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setEditTriggers(QAbstractItemView::SelectedClicked);
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->setDragEnabled(true);
    this->setAcceptDrops(true);
    this->setDropIndicatorShown(true);
	this->setModel(_model);
    this->setRootIndex(_model->index(assetRoot));
	this->setHeaderHidden(true);
	this->hideColumn(1);
	this->hideColumn(2);
	this->hideColumn(3);
    connect(this, &QTreeView::doubleClicked, this, &res_tree::OnItemDoubleClicked);
    connect(this, &QTreeView::customContextMenuRequested, this, &res_tree::OnCustomContextMenu);
    connect(this, &QTreeView::clicked, this, &res_tree::OnItemClicked);
    this->setEditTriggers(QAbstractItemView::SelectedClicked);
}

res_tree::~res_tree()
{
	

}

void res_tree::showEvent(QShowEvent *event)
{
	QTreeView::showEvent(event);
	
}

void res_tree::createContexMenus()
{
    contexMenu = new QMenu();

    actionCreateFile = new QAction(contexMenu);
    actionCreateFile->setText(QObject::tr("create"));
    menuCreateFile = new QMenu();

    actionCreateMat = new QAction(menuCreateFile);
    actionCreateMat->setText(QObject::tr("Create Material"));
    connect(actionCreateMat, &QAction::triggered, this, &res_tree::OnCreateMaterial);
    menuCreateFile->addAction(actionCreateMat);

    actionCreateDir = new QAction(menuCreateFile);
    actionCreateDir->setText(QObject::tr("Create Dir"));
    connect(actionCreateDir, &QAction::triggered, this, &res_tree::OnAddDir);
    menuCreateFile->addAction(actionCreateDir);

    actionCreateFile->setMenu(menuCreateFile);
    contexMenu->addMenu(menuCreateFile);

    actionImportAssets = new QAction();
    actionImportAssets->setText(QObject::tr("Import Assets"));
    connect(actionImportAssets, &QAction::triggered, this, &res_tree::OnImportAssets);
    contexMenu->addAction(actionImportAssets);

    actionShowinExpl = new QAction();
    actionShowinExpl->setText(QObject::tr("ShowInExplorer"));
    connect(actionShowinExpl, &QAction::triggered, this, &res_tree::OnShowInExplorer);
    contexMenu->addAction(actionShowinExpl);

    actionDeletefile = new QAction();
    actionDeletefile->setText(QObject::tr("Delete"));
    connect(actionDeletefile, &QAction::triggered, this, &res_tree::OnRemoveItem);
    contexMenu->addAction(actionDeletefile);
}

void res_tree::OnAddDir()
{

}

void res_tree::OnCreateMaterial()
{

}

void res_tree::OnShowInExplorer()
{
    QModelIndexList indexList = this->selectedIndexes();
    if (indexList.size() == 0)
    {
        QString path = QDir::toNativeSeparators(assetRoot);
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
        return;
    }
    for(QModelIndex var:indexList)
    {
        if (_model->fileInfo(var).isDir())
        {
            QString path = QDir::toNativeSeparators(_model->fileInfo(var).absoluteFilePath());
            QDesktopServices::openUrl(QUrl::fromLocalFile(path));
        }
        else
        {
            QString path = QDir::toNativeSeparators(_model->fileInfo(var).absoluteFilePath());
            QProcess process;
            process.startDetached(("explorer.exe /select," + path));
        }
    }
}

void res_tree::OnRemoveItem()
{

}

void res_tree::OnImportAssets()
{
     QModelIndexList indexList = this->selectedIndexes();
     for(QModelIndex var:indexList)
     {
         if (_model->fileInfo(var).isDir())
         {
             QString path = QDir::toNativeSeparators(_model->fileInfo(var).absoluteFilePath());
             asset_mgr::get_instance()->import_assets(path.toStdString());
         }
     }

}

void res_tree::OnItemClicked(const QModelIndex index)
{
    /*
     auto index = currentIndex();
            if (index.isValid() && resTreeModel->fileInfo(index).isFile())
            {
                QString ex = resTreeModel->fileInfo(index).suffix().toLower();
                QString baseName = resTreeModel->fileInfo(index).baseName();
                std::string resPath = Utils::qstr2str(resTreeModel->fileInfo(index).absoluteFilePath());
                //QString textureStr = ".png.tga.jpg.jpeg";
                if (ex == "fbx")
                {
                    EventData* evt = new EventData();
                    evt->stringList.push_back("fbxMeta");
                    for (auto var : selectedIndexes())
                    {
                        std::string tmpPath = Utils::qstr2str(resTreeModel->fileInfo(var).absoluteFilePath());
                        QString tmpex = resTreeModel->fileInfo(var).suffix().toLower();
                        QString tmpbaseName = resTreeModel->fileInfo(var).baseName();
                        if (resTreeModel->fileInfo(var).isFile() && tmpex == "fbx" )
                        {
                            evt->stringList.push_back(tmpPath);
                        }

                    }
                    EventMgr::GetInstance()->emitEvent(eGlobalEventType::SwitchEditorInspector, evt);
                }
            }
     */
}

void res_tree::OnItemDoubleClicked(const QModelIndex index)
{
    if(!_model->fileInfo(index).isDir())
    {
        QString path = _model->fileInfo(index).absoluteFilePath();
        QFileInfo info(path);
        QString suffix_filter = "fbx.prefab.mat.scene";
        if( suffix_filter.split(".").contains(info.suffix().toLower()))
        {
            if ("fbx"==info.suffix().toLower())
            {
                if (info.baseName().contains("@"))//fbx动画
                    return;
            }
           //EventMgr::GetInstance()->emitEvent(eGlobalEventType::OpenAsset,new EventData(relativePath));
        }

    }
}

void res_tree::OnCustomContextMenu(const QPoint &p)
{
    actionCreateFile->setEnabled(true);
    actionShowinExpl->setEnabled(true);
    actionDeletefile->setEnabled(true);
    //actionReimportFbx->setEnabled(false);
    QModelIndexList indexList = this->selectedIndexes();
    if (indexList.size() == 0)
    {
        actionCreateFile->setEnabled(true);
        actionShowinExpl->setEnabled(true);
        actionDeletefile->setEnabled(false);
        //actionReimportFbx->setEnabled(false);
        contexMenu->exec(QCursor::pos());
        return;
    }
    bool hasDir = false;
    for(QModelIndex var:indexList)
    {
        if (_model->fileInfo(var).isDir())
        {
            hasDir = true;
        }
    }
    if (hasDir)
    {

    }
    contexMenu->exec(QCursor::pos());
}

}
}
