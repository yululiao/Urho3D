#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <exception>
#include <set>
#include <stdio.h>
#include <stdlib.h>

#include <QString>
#include <QDir>
#include <QFile>
#include <QCryptographicHash>
#include <QUuid>
#include "translate.h"

#include "Urho3D/Math/Ray.h"
#include "Urho3d/Graphics/StaticModel.h"
#include "Urho3d/Graphics/AnimatedModel.h"
#include "Urho3D/Graphics/Geometry.h"
#include "Urho3D//Graphics/VertexBuffer.h"
#include "Urho3D/Graphics/IndexBuffer.h"

using namespace Urho3D;

namespace urho3d
{
namespace editor
{
class Utils
{
public:
	static bool file_exist(const std::string& filePath)
	{
		QFileInfo fileInfo(str2qstr(filePath));
		if (fileInfo.isFile())
		{
			return true;
		}
		return false;
	}

	static bool dir_exist(const std::string& dirPath)
	{
		bool result = false;
		QDir dir;
		result = dir.exists(str2qstr(dirPath));
		return result;
	}

	static bool remove_file(const std::string& filePath)
	{
		bool result = false;
		if (file_exist(filePath))
		{
			result = QFile::remove(str2qstr(filePath));
		}

		return result;
	}

	static bool delete_dir(const std::string& path)
	{
		QString qPath = str2qstr(path);
		if (qPath.isEmpty())
		{
			return false;
		}

		QDir dir(qPath);
		if (!dir.exists())
		{
			return true;
		}
		dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
		QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
		foreach(QFileInfo file, fileList)
		{ //遍历文件信息
			if (file.isFile())
			{ // 是文件，删除
				file.dir().remove(file.fileName());
			}
			else
			{ // 递归删除
				delete_dir(qstr2str(file.absoluteFilePath()));
			}
		}
		return dir.rmpath(dir.absolutePath()); // 删除文件夹
	}

	static bool create_file(const std::string& fullpath)
	{
		QString qfullPath = str2qstr(fullpath);
		bool result = false;
		QFile file(qfullPath);
		if (!file.exists())
		{
			result = file.open(QIODevice::ReadWrite | QIODevice::Text);
			file.close();
		}
		return result;
	}

	static bool makedir(const std::string& dirpath)
	{
		QString qDirPath = str2qstr(dirpath);
		QDir dir;
		bool result = false;
		if (!dir.exists(qDirPath))
			result = dir.mkdir(qDirPath);
		return result;

	}
	static std::string get_auto_dirname(const std::string& parentPath, const std::string& pre)
	{
		QString qParentPath = str2qstr(parentPath);
		QString qPre = str2qstr(pre);
		int num = 1;
		QString dirName = qParentPath + QString("/") + qPre + QString::number(num);
		QDir dir(dirName);
		while (dir.exists())
		{
			num++;
			dirName = qParentPath + QString("/") + qPre + QString::number(num);
			dir.setPath(dirName);
		}
		return qstr2str(dirName);
	}

	static std::string mkdirs(const std::string& path)
	{
		QString qPath = str2qstr(path);
		QDir dir(qPath);
		if (dir.exists(qPath))
		{
			return path;
		}
		std::string parentDir = mkdirs(path.substr(0, qPath.lastIndexOf('/')));
		std::string dirname = path.substr(qPath.lastIndexOf('/') + 1);
		QDir parentPath(str2qstr(parentDir));
		if (dirname != "")
			parentPath.mkpath(str2qstr(dirname));
		return parentDir + "/" + dirname;
	}

	static std::string get_auto_filename(const std::string& parentPath, const std::string& pre, const std::string& ext)
	{
		int num = 1;
		std::string fileName = parentPath + "/" + pre + qstr2str(QString::number(num)) + ext;
		QFile file(str2qstr(fileName));
		while (file.exists())
		{
			num++;
			fileName = parentPath + "/" + pre + qstr2str(QString::number(num)) + ext;
			file.setFileName(str2qstr(fileName));
		}
		return fileName;
	}

	static QString str2qstr(const std::string str)
	{
		QString result = QString::fromLocal8Bit(str.data());

		return result;

	}

	static std::string qstr2str(const QString qstr)
	{
		QByteArray cdata = qstr.toLocal8Bit();
		return std::string(cdata);
	}

	static void get_files_in_dir(const std::string& path, std::vector<std::string>& files)
	{

		QDir dir(str2qstr(path));

		foreach(QFileInfo mfi, dir.entryInfoList())
		{
			if (mfi.isFile())
			{
				files.push_back(qstr2str(mfi.absoluteFilePath()));
			}
			else
			{
				if (mfi.fileName() == "." || mfi.fileName() == "..")continue;
				get_files_in_dir(qstr2str(mfi.absoluteFilePath()), files);
			}
		}
	}

	static void get_dirs_in_dir(const std::string& path, std::vector<std::string>& files)
	{
		QDir dir(str2qstr(path));

		foreach(QFileInfo mfi, dir.entryInfoList())
		{
			if (mfi.isDir())
			{
				if (mfi.fileName() == "." || mfi.fileName() == "..")continue;
				files.push_back(qstr2str(mfi.absoluteFilePath()));
				get_dirs_in_dir(qstr2str(mfi.absoluteFilePath()), files);
			}
		}
	}

	static void get_files_notrecur(const std::string& path, std::vector<std::string>& files)
	{
		QDir dir(str2qstr(path));

		foreach(QFileInfo mfi, dir.entryInfoList())
		{
			if (mfi.isFile())
			{
				files.push_back(qstr2str(mfi.absoluteFilePath()));
			}
		}
	}

	static void get_dirs_notrecur(const std::string& path, std::vector<std::string>& files)
	{
		QDir dir(str2qstr(path));

		foreach(QFileInfo mfi, dir.entryInfoList())
		{
			if (mfi.isDir() && mfi.fileName() != "." && mfi.fileName() != "..")
			{
				files.push_back(qstr2str(mfi.absoluteFilePath()));
			}
		}
	}


	static void get_files_and_dir_notrecur(const std::string& path, std::vector<std::string>& files)
	{
		QDir dir(str2qstr(path));

		foreach(QFileInfo mfi, dir.entryInfoList())
		{
			if (mfi.isFile())
			{
				files.push_back(qstr2str(mfi.absoluteFilePath()));
			}
			if (mfi.isDir() && mfi.fileName() != "." && mfi.fileName() != "..")
			{
				files.push_back(qstr2str(mfi.absoluteFilePath()));
			}
		}
	}

	static std::string number_to_string(float number)
	{
		QString str = QString::number(number);

		return qstr2str(str);
	}

	static std::string get_file_md5(const std::string& str_sourceFilePath)
	{
		QString sourceFilePath = str2qstr(str_sourceFilePath);
		QFile sourceFile(sourceFilePath);
		qint64 fileSize = sourceFile.size();
		const qint64 bufferSize = 10240;

		if (sourceFile.open(QIODevice::ReadOnly))
		{
			char buffer[bufferSize];
			int bytesRead;
			int readSize = qMin(fileSize, bufferSize);

			QCryptographicHash hash(QCryptographicHash::Md5);

			while (readSize > 0 && (bytesRead = sourceFile.read(buffer, readSize)) > 0)
			{
				fileSize -= bytesRead;
				hash.addData(buffer, bytesRead);
				readSize = qMin(fileSize, bufferSize);
			}

			sourceFile.close();
			return qstr2str(QString(hash.result().toHex()));
		}
		return std::string();
	}

	static std::string gen_guid()
	{
		QUuid id = QUuid::createUuid();
		QString strId = id.toString();

		return qstr2str(strId);

	}

	static std::string get_file_name(const std::string& fullpath)
	{
		QString qstr = Utils::str2qstr(fullpath);
		QFileInfo info(qstr);

		return Utils::qstr2str(info.fileName());
	}
	static std::string get_file_ext(const std::string& fullpath)
	{
		QString qstr = Utils::str2qstr(fullpath);
		QFileInfo info(qstr);
		return Utils::qstr2str("." + info.suffix());
	}
	static std::string get_base_name(const std::string& fullpath)
	{
		QString qstr = Utils::str2qstr(fullpath);
		QFileInfo info(qstr);
		return Utils::qstr2str(info.baseName());
	}

	static std::string read_text_file(const std::string& fullPath)
	{
		std::ifstream in(fullPath);
		std::string content = std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		in.close();
		return content;
	}
	static void write_text_file(const std::string& fullPath, const std::string& content, bool overite = false)
	{
		if (Utils::file_exist(fullPath))
		{
			if (!overite)
			{
				throw ("Utils::writeTextFile: file exist!");
				return;
			}
		}
		std::ofstream outfile;
		outfile.open(fullPath);
		if (!outfile.is_open())
		{
			//log_err("Editor", "Write file failed,can't open <%s>", filePath1.c_str());
			return;
		}
		outfile << content << std::endl;
		outfile.close();
	}
	/// <summary>
	/// 执行一个命令行
	/// </summary>
	/// <param name="cmd">命令行</param>
	/// <param name="pRetMsg">执行返回值</param>
	/// <param name="msg_len"></param>
	/// <returns></returns>
	static int _system(const char* cmd, char* pRetMsg, int msg_len)
	{
		FILE* fp;
		char* p = NULL;
		int res = -1;
		if (cmd == NULL || pRetMsg == NULL || msg_len < 0)
		{
			printf("Param Error!\n");
			return -1;
		}
		if ((fp = _popen(cmd, "r")) == NULL)
		{
			printf("Popen Error!\n");
			return -2;
		}
		else
		{
			memset(pRetMsg, 0, msg_len);
			//get lastest result  
			while (fgets(pRetMsg, msg_len, fp) != NULL)
			{
				printf("Msg:%s", pRetMsg); //print all info  
			}

			if ((res = _pclose(fp)) == -1)
			{
				printf("close popenerror!\n");
				return -3;
			}
			//pRetMsg[strlen(pRetMsg) - 1] = '\0';
			return 0;
		}
	}
	static std::vector<std::string> splite_str(const std::string& str,const std::string& sp)
	{
		std::vector<std::string> result;
		QString q_str = str2qstr(str);
		auto qstrlist = q_str.split(str2qstr(sp));
		for (auto item : qstrlist)
		{
			result.push_back(qstr2str(item));
		}

		return result;

	}

	static Node* intersectObj(Ray& ray, Node* pnode, float& dis)
	{
		Node* result = nullptr;
		std::map<float, Node*> out;
		intersectObj(ray, pnode, out);
		if (out.size() == 0)
			return result;
		result = out.begin()->second;
		dis = out.begin()->first;
		return result;
	}
	static void intersectObj(Ray& ray, Node* pnode, std::map<float, Node*>& out)
	{
		StaticModel* stmodel = pnode->GetComponent<StaticModel>();
		AnimatedModel* animodel = pnode->GetComponent<AnimatedModel>();
		Model* tmodel = nullptr;
		if(stmodel)
			tmodel = stmodel->GetModel();
		if (animodel)
			tmodel = animodel->GetModel();
		if (tmodel)
		{
			unsigned int geoNum = tmodel->GetNumGeometries();
			float dis = M_INFINITY;
			std::string name = pnode->GetName().CString();
			for (int i = 0; i < geoNum; i++)
			{
				Geometry* geo = tmodel->GetGeometry(i, 0);
				VertexBuffer* vb = geo->GetVertexBuffer(0);
				unsigned vitemsize = vb->GetVertexSize();
				void* vbdata = vb->GetShadowData();
				void* vidata = geo->GetIndexBuffer()->GetShadowData();
				unsigned indexSize = geo->GetIndexBuffer()->GetIndexSize();
				unsigned indexCount = geo->GetIndexBuffer()->GetIndexCount();
				Matrix3x4 nodeRot = pnode->GetWorldTransform();
				Ray t_ray = ray;
				t_ray.hitTransform_ = nodeRot;
				float t_dis = t_ray.HitDistance(vbdata, vitemsize, vidata, indexSize, 0, indexCount);
				if (t_dis < dis)
				{
					dis = t_dis;
				}
			}
			if (dis < M_INFINITY && out.find(dis) == out.end())
			{
				out[dis] = pnode;
			}
		}
		auto& children = pnode->GetChildren();
		for (auto it = children.Begin(); it != children.End(); it++)
		{
			intersectObj(ray, *it, out);
		}

	}

};
}
}