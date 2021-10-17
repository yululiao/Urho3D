#pragma once
#include <QObject>
#include <QMap>
#include <QFileSystemWatcher>
#include <QSet>

#include "event_emitter.h"


namespace urho3d
{
namespace editor
{

struct FileWatchEvent
{
	enum
	{
		AddFilesOrDirs,//num(int),addPath1(string),addPath2,...,addPathN
		DeleteFilesOrDirs,//num(int),deletePath1(string),deletePath2,...,deletePathN
		RenameFileOrDir, //fromFilePath(string),toFilepath,(string)
		ModifyFile,
		RevertFile,
		COUNT,
	};
};

class file_system_watcher : public QObject
{
	//Q_OBJECT
public:
	void add_watch_path(QString path);
	event_emitter* eventEmitter;
	void handleDirUpdate();
	void handleFileUpdate();

public slots:
	file_system_watcher(QObject* parent = 0);
	void directoryUpdated(const QString& path);  // 目录更新时调用，path是监控的路径
	void fileUpdated(const QString& path);   // 文件被修改时调用，path是监控的路径

private:
	//static FileSystemWatcher *instance; // 单例
	QFileSystemWatcher* watcher;  // QFileSystemWatcher变量
	QMap<QString, QStringList> currentContentsMap; // 当前每个监控的内容目录列表
	QMap<QString, bool> currentWatchingMap;

	QSet<QString> dirUpdateSet;
	QSet<QString> fileUpdateSet;

};

}
}
