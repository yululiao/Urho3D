#include <fstream>
#include <sstream>
#include "HistoryMgr.h"
#include "EditorApp.h"
#include "Urho3D/Resource/JSONValue.h"
#include "Urho3D/Resource/JSONFile.h"
#include "Urho3D/IO/Serializer.h"

namespace Urho3DEditor
{
HistoryMgr::HistoryMgr()
{
	_history_data = new ProjHistory(EditorApp::GetInstance()->GetContext());
	open();
}

HistoryMgr::~HistoryMgr()
{

}


void HistoryMgr::add_project(const String& path)
{

	_history_data->_history.Insert(0, path);

}

void HistoryMgr::delete_history(const String& path)
{
	//_history_data->_hitory.Pop();
	int pos = -1;
	for (int i = 0; i < _history_data->_history.Size(); i++)
	{
		if (_history_data->_history[i] == path)
		{
			pos = i;
		}
	}
	if (pos >= 0)
		_history_data->_history.Erase(pos);
}

void HistoryMgr::save()
{
	JSONValue json;
	_history_data->SaveJSON(json);
	JSONFile j_file(EditorApp::GetInstance()->GetContext());
	j_file.GetRoot() = json;

	String j_str = j_file.ToString();
	std::string std_j_str = j_str.CString();

	std::ofstream of(_history_file.CString());
	of << std_j_str;
	of.close();

}

void HistoryMgr::open()
{

	JSONFile j_file(EditorApp::GetInstance()->GetContext());
	//将文件读入到ostringstream对象buf中
	std::ifstream ifile(_history_file.CString());
	std::ostringstream buf;
	char ch;
	while (buf&&ifile.get(ch))
		buf.put(ch);
	std::string json_str = buf.str();
	j_file.FromString(json_str.c_str());

	_history_data->LoadJSON(j_file.GetRoot());


	int test = 10;

}

StringVector HistoryMgr::get_history_list()
{
	return _history_data->_history;
}


}