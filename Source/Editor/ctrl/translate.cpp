#include "translate.h"

translator* translator::_translator = nullptr;

translator* translator::getInstance()
{
    if(_translator == nullptr)
    {
        _translator = new translator();
    }

    return _translator;
}

void translator::setLocation(const std::string& location)
{
    _location = location;
    //todo
    //load translation file and set the _str_map
}

QString translator::dytr(const char* str)
{
    if (_str_map.find(std::string(str)) != _str_map.end()){
         return QString(_str_map[std::string(str)].c_str());
    }
    else {
       return  QString(str);
    }
}

QString translator::dytr(const std::string& str)
{
    if (_str_map.find(std::string(str)) != _str_map.end()){
         return QString(_str_map[str].c_str());
    }
    else {
       return  QString(str.c_str());
    }
}


