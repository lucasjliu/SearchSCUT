#ifndef _DOCS_H_
#define _DOCS_H_

#include "Config.h"

#include <string>
#include <vector>
#include <fstream>

class Docs
{
private:
	_DOC_VTR _docs;
	std::string _filePath;
public:
	Docs(std::string filePath);
	Docs(){_docs = _DOC_VTR();}
	_DOC_ID add(_DOC_INFO);
	_DOC_INFO get_info(_DOC_ID);
	bool save();
	unsigned int size(){return _docs.size();}
};

#endif