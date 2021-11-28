#include <QIODevice>
#include <QDataStream>
#include <QMimeData>
#include <QCoreApplication>
#include "NodeTreeModel.h"
#include "ctrl/scene_ctrl.h"


namespace urho3d
{
namespace editor
{

NodeThreeModel::NodeThreeModel(QObject* parent)
{
    global_event::get_instance()->register_event(eGlobalEventType::BeginInsertNode,MakeDelegate(this,&NodeThreeModel::onBeginInsertNode));
    global_event::get_instance()->register_event(eGlobalEventType::EndInsertNode,MakeDelegate(this,&NodeThreeModel::onEndInsertNode));

}
NodeThreeModel::~NodeThreeModel()
{
    global_event::get_instance()->remove_event(eGlobalEventType::BeginInsertNode,MakeDelegate(this,&NodeThreeModel::onBeginInsertNode));
    global_event::get_instance()->remove_event(eGlobalEventType::EndInsertNode,MakeDelegate(this,&NodeThreeModel::onEndInsertNode));
}
void NodeThreeModel::SetRootNode(Node* node)
{
    _rootNode = node;
    this->Reset();
}

QModelIndex NodeThreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!_rootNode || row < 0 || column < 0)
            return QModelIndex();
    Node* parentNode = nodeFromIndex(parent);
    Node* childNode = parentNode->GetChild(row);//parentNode->children.value(row);
    if (!childNode)
        return QModelIndex();
    return createIndex(row, column, childNode);
}
QModelIndex NodeThreeModel::parent(const QModelIndex &child) const
{
    Node* node = nodeFromIndex(child);
    if (!node)
        return QModelIndex();
     Node* parentNode = node->GetParent();
    if (!parentNode)
        return QModelIndex();
    Node *grandNode = parentNode->GetParent();
    if (!grandNode)
        return QModelIndex();
    //int row = grandNode->children.indexOf(parentNode);
    int row = grandNode->GetChildren().IndexOf(SharedPtr<Node>(parentNode));
    return createIndex(row, 0, parentNode);

}
int NodeThreeModel::rowCount(const QModelIndex &parent) const
{
    Node* parentNode = nodeFromIndex(parent);
    if (!parentNode)
        return 0;
    return parentNode->GetChildren().Size();
}
int NodeThreeModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}
QVariant NodeThreeModel::data(const QModelIndex &index, int role) const
{
    Node* node = nodeFromIndex(index);
    if (!node)
        return QVariant();
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if (index.column() == 0)
        {
            return QString(node->GetName().CString());
        }
    }
    return QVariant();
}
QVariant NodeThreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}
bool NodeThreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Node* node = nodeFromIndex(index);
    if (node && QString(node->GetName().CString()) != value.toString())
    {
        //result = node->ReName(value.toString());
       node->SetName(value.toString().toStdString().c_str());
    }
        return true;
}
Qt::ItemFlags NodeThreeModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index);
}
Qt::DropActions NodeThreeModel::supportedDropActions() const
{
    return Qt::MoveAction;
}
Qt::DropActions NodeThreeModel::supportedDragActions() const
{
    return Qt::MoveAction;
}
static const char G_NodeMimeType[] = "Urho3deditor/NodeMime";
QStringList NodeThreeModel::mimeTypes() const
{
    return QStringList() << G_NodeMimeType;
}
QMimeData* NodeThreeModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData;
    QByteArray data; //a kind of RAW format for datas
    QDataStream stream(&data, QIODevice::WriteOnly);
    QList<Node *> nodes;
    for(const QModelIndex &index:indexes)
    {
        Node *node = nodeFromIndex(index);
        if (!nodes.contains(node))
            nodes << node;
    }
    stream << QCoreApplication::applicationPid();
    stream << nodes.count();
    for(Node *node: nodes)
    {
        stream << reinterpret_cast<qlonglong>(node);
    }
    mimeData->setData(G_NodeMimeType, data);//多选拖拽
    return mimeData;
}
bool NodeThreeModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex & parent)
{
    return QAbstractItemModel::dropMimeData(data,action,row,column,parent);
}

void NodeThreeModel::Reset()
{
    beginResetModel();
    endResetModel();
}

Node* NodeThreeModel::getRoot()
{
    return _rootNode;
}
Node* NodeThreeModel::nodeFromIndex(const QModelIndex &index) const
{
    if (index.isValid())
        {
            return static_cast<Node*>(index.internalPointer());
        }
        else
        {
            return _rootNode;
        }
}

void NodeThreeModel::onBeginInsertNode(event_data* data)
{
    int count = scene_ctrl::get_inatance()->getRoot()->GetChildren().Size();
    beginInsertRows(QModelIndex(),count,count);
}


void NodeThreeModel::onEndInsertNode(event_data* data)
{
    endInsertRows();
}


}
}
