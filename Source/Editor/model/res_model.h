#ifndef RES_MODEL_H
#define RES_MODEL_H

#include <QFileSystemModel>
#include <QModelIndex>
#include <QMimeData>

namespace urho3d
{
namespace editor
{

class res_model: public QFileSystemModel
{
public:
    res_model(QObject* parent = nullptr);
    ~res_model() override;
    QMimeData *mimeData(const QModelIndexList &indexes) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
};


}

}

#endif // RES_MODEL_H
