#pragma once
#include <string>
#include <map>
#include <QString>

class translator
{
public:
	
    translator(){}
    ~translator(){}
    static translator* getInstance();
    QString dytr(const char* str);
    QString dytr(const std::string& str);
    void setLocation(const std::string& location);
private:
	static translator* _translator;
    std::string _location;
    std::map<std::string,std::string> _str_map;
};
