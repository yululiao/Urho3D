#include <QDir>
#include <QFileInfo>
#include <qDebug>
#include "file_system_watcher.h"
#include "Utils.h"


namespace urho3d
{
namespace editor
{

//FileSystemWatcher* FileSystemWatcher::instance = NULL;

file_system_watcher::file_system_watcher(QObject* parent)
	: QObject(parent)
{
	eventEmitter = new event_emitter();
	watcher = new QFileSystemWatcher();
	connect(watcher, SIGNAL(directoryChanged(QString)), this, SLOT(directoryUpdated(QString)));
	connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(fileUpdated(QString)));
}

// 监控文件或目录
void file_system_watcher::add_watch_path(QString path)
{
	qDebug() << QString("Add to watch: %1").arg(path);

	// 添加监控路径
	this->watcher->addPath(path);
	currentWatchingMap[path] = true;
	// 如果添加路径是一个目录，保存当前内容列表
	QFileInfo file(path);
	if (file.isDir())
	{
		const QDir dirw(path);
		this->currentContentsMap[path] = dirw.entryList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
		auto dirs = dirw.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
		foreach(QFileInfo childDir, dirs)
		{
			auto p = childDir.absoluteFilePath();
			add_watch_path(childDir.absoluteFilePath());
		}
	}

}
void file_system_watcher::handleDirUpdate()
{
	for (auto path : dirUpdateSet)
	{
		// 比较最新的内容和保存的内容找出区别(变化)
		QStringList currEntryList = currentContentsMap[path];
		const QDir dir(path);

		QStringList newEntryList = dir.entryList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);

		QSet<QString> newDirSet = QSet<QString>::fromList(newEntryList);
		QSet<QString> currentDirSet = QSet<QString>::fromList(currEntryList);

		// 添加了文件
		QSet<QString> newFiles = newDirSet - currentDirSet;
		QStringList newFile = newFiles.toList();

		// 文件已被移除
		QSet<QString> deletedFiles = currentDirSet - newDirSet;
		QStringList deleteFile = deletedFiles.toList();

		// 更新当前设置
		currentContentsMap[path] = newEntryList;

		if (!newFile.isEmpty() && !deleteFile.isEmpty())
		{
			// 文件/目录重命名
			if ((newFile.count() == 1) && (deleteFile.count() == 1))
			{
				qDebug() << QString("File Renamed from %1 to %2").arg(deleteFile.first()).arg(newFile.first());
				auto file = newFile[0];
				auto fullPath = path + "/" + file;
				add_watch_path(fullPath);
				eventEmitter->emit_event(FileWatchEvent::RenameFileOrDir, new event_data(Utils::qstr2str(fullPath)));
			}
		}
		else
		{
			// 添加新文件/目录至Dir
			if (!newFile.isEmpty())
			{
				qDebug() << "New Files/Dirs added: " << newFile;

				foreach(QString file, newFile)
				{
					auto fullPath = path + "/" + file;
					add_watch_path(fullPath);
					eventEmitter->emit_event(FileWatchEvent::AddFilesOrDirs, new event_data(Utils::qstr2str(fullPath)));
				}
			}

			// 从Dir中删除文件/目录
			if (!deleteFile.isEmpty())
			{
				qDebug() << "Files/Dirs deleted: " << deleteFile;

				foreach(QString file, deleteFile)
				{
					if (currentContentsMap.contains(file))
					{
						currentContentsMap.remove(file);
						auto fullPath = path + "/" + file;
						eventEmitter->emit_event(FileWatchEvent::DeleteFilesOrDirs, new event_data(Utils::qstr2str(fullPath)));
					}
				}
			}

			if (newFile.isEmpty() && deleteFile.isEmpty())
			{
				eventEmitter->emit_event(FileWatchEvent::RevertFile, new event_data(Utils::qstr2str(path)));
			}
		}
	}
}
void file_system_watcher::handleFileUpdate()
{
	for (auto path : fileUpdateSet)
	{
		QFileInfo file(path);
		QString strPath = file.absolutePath();
		QString strName = file.fileName();

		qDebug() << QString("The file %1 at path %2 is updated").arg(strName).arg(strPath);
		eventEmitter->emit_event(FileWatchEvent::ModifyFile, new event_data(Utils::qstr2str(path)));
	}
}

// 只要任何监控的目录更新（添加、删除、重命名），就会调用。
void file_system_watcher::directoryUpdated(const QString& path)
{
	//qDebug() << QString("Directory updated: %1").arg(path);
	if (!dirUpdateSet.contains(path))
	{
		dirUpdateSet.insert(path);
	}

}

// 文件修改时调用
void file_system_watcher::fileUpdated(const QString& path)
{
	if (!fileUpdateSet.contains(path))
	{
		fileUpdateSet.insert(path);
	}
}

}//editor
}