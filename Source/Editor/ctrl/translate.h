#pragma once
#include <QObject>

class translator:public QObject
{
	Q_OBJECT
public:
	
	translator()
	{
	}
	virtual QString translate(const char* str);
	virtual QString translate(const std::string& str);
	static void set_translator(translator* trans);
	static QString dytr(const char* str);
	static QString dytr(const std::string& str);
private:
	static translator* _translator;
};
