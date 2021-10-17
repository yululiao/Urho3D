#include "translate.h"

translator* translator::_translator = nullptr;
QString translator::translate(const char* str)
{
	return "";
}

QString translator::translate(const std::string& str)
{
	return "";
}

void translator::set_translator(translator* trans)
{
	_translator = trans;
}

QString translator::dytr(const char* str)
{
	return _translator->translate(str);
}

QString translator::dytr(const std::string& str)
{
	return _translator->translate(str);
}
