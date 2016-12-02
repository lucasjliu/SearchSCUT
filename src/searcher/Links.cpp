#include "Links.h"

extern const _WORD_ID noid;

extern const _URL nourl;

Links::Links(std::string filePath)
	:_filePath(filePath)
{
	_links = _URL_MAP();
	std::ifstream fsLinks(filePath);
	if(fsLinks.fail())
		return;
	std::string line;
	_URL url;
	_DOC_ID docId;
	while(getline(fsLinks, line))
	{
		std::stringstream tmpStream(line);
		if(!(tmpStream >> url))
			continue;
		if(!(tmpStream >> docId))
			continue;
		_links[url] = docId;
	}
	fsLinks.close();
}
/*
bool Links::save()
{
	std::ofstream fsLinks(_filePath);
	if(fsLinks.fail())
		return false;
	while(!_links.empty())
	{
		fsLinks << _links.begin()->first << " "
			<< _links.begin()->second << std::endl;
		_links.erase(_links.begin());
	}
	return true;
}*/
_DOC_ID Links::get_id(_URL url)
{
	_URL_MAP_ITR itr = _links.find(url);
	if(itr == _links.end())
		return noid;
	return itr->second;
}
_URL Links::get_url(_DOC_ID docId)
{
	_URL_MAP_ITR itr = _links.begin();
	_URL_MAP_ITR end = _links.end();
	_URL ret = nourl;
	while(itr != end)
	{
		if(itr->second == docId)
		{
			ret = itr->first;
			break;
		}
		itr++;
	}
	return ret;
}