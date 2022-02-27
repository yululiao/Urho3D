#ifndef NODETREEMODEL_H
#define NODETREEMODEL_H

#include <QAbstractItemModel>
#include "Urho3D/Scene/Node.h"
#include "ctrl/global_event.h"

using namespace  Urho3D;

namespace urho3d
{
namespace editor
{

class NodeThreeModel : public QAbstractItemModel
{
public:
    NodeThreeModel(QObject* parent = 0);
    ~NodeThreeModel();
    void SetRootNode(Node* node);

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Qt::DropActions supportedDropActions() const Q_DECL_OVERRIDE;
    Qt::DropActions supportedDragActions() const Q_DECL_OVERRIDE;
    QStringList mimeTypes() const Q_DECL_OVERRIDE;
    QMimeData *mimeData(const QModelIndexList &indexes) const Q_DECL_OVERRIDE;
    bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex & parent) override;
    void remove(const QModelIndex& index);
    //void insertNode(const QModelIndex& index,int row);
    void Reset();
    Node* getRoot();
    Node* nodeFromIndex(const QModelIndex &index) const;

protected:
    void onBeginInsertNode(event_data* data);
    void onEndInsertNode(event_data* data);

private:
    Node* _rootNode = nullptr;
};
}
}

#endif // NODETREEMODEL_H
