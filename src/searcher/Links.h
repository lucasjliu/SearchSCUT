#ifndef _LINKS_H_
#define _LINKS_H_

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include "Docs.h"

class Links
{
private:
	_URL_MAP _links;
	std::string _filePath;
public:
	Links(std::string filePath);
	Links(){_links = _URL_MAP();}
	//bool save();
	_DOC_ID get_id(_URL url);
	_URL get_url(_DOC_ID docId);
	unsigned int size(){return _links.size();}
};


#endif