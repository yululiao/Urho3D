#include <QMessageBox>
#include "res_model.h"

namespace urho3d
{
namespace editor
{

res_model::res_model(QObject* p)
    :QFileSystemModel(p)
{

}

res_model::~res_model()
{

}

bool res_model::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (role == Qt::ItemDataRole::EditRole)
    {
        QString oldName = index.data(Qt::ItemDataRole::DisplayRole).toString();
        QString newName = value.toString();
        if (oldName != newName)
        {
            if (newName == "")
            {
                QMessageBox::warning(nullptr, "warning", QStringLiteral("文件名不能为空"));
                return true;
            }
            if (newName.contains(" ") || newName.contains("/") || newName.contains("\\"))
            {
                QMessageBox::warning(nullptr, "warning", QStringLiteral("文件名包含非法字符"));
                return true;
            }
            QFileSystemModel::setData(index,value,role);
        }
    }
    return true;
}


QMimeData* res_model::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = QFileSystemModel::mimeData(indexes);
    QByteArray data;

    QDataStream stream(&data, QIODevice::WriteOnly);

    if (indexes.count() > 0)
    {
        mimeData->setData("DragAssetToScene",this->fileInfo(indexes.at(0)).absoluteFilePath().toLocal8Bit());
    }

    return mimeData;
}

}
}
