#include "Docs.h"

extern const _DOC_INFO nodoc;

Docs::Docs(std::string filePath)
	:_filePath(filePath)
{
	_docs = _DOC_VTR();
	_IN_FILE fsDocs(filePath);
	if(fsDocs.fail())
		return;
	string line;
	while(getline(fsDocs, line))
	{
		add(line);
	}
	fsDocs.close();
}
bool Docs::save()
{
	ofstream fsDocs(_filePath);
	if(fsDocs.fail())
		return false;
	while(!_docs.empty())
	{
		fsDocs<<_docs.front()<<std::endl;
		_docs.erase(_docs.begin());
	}
	return true;
}
_DOC_ID Docs::add(_DOC_INFO name)
{
	_docs.push_back(name);
	return _docs.size() - 1;
}
_DOC_INFO Docs::get_info(_DOC_ID docId)
{
	if(docId > _docs.size())
		return nodoc;
	return _docs[docId];
}