#include "Query.h"

Query::Query()
{
}
Query::Query(string query, int id)
	:_query(query), _qid(id)
{
}
string Query::get_query() const
{
	return _query;
}
int Query::get_id() const
{
	return _qid;
}
bool Query::find_doc(_DOC_ID docId)
{
	return _valMap.find(docId) != _valMap.end();
}
void Query::add_doc(_DOC_ID docId, double val)
{
	_valMap[docId] = val;
	_DOC_LIST_ITR itr = _docList.begin();
	int loop = 0;
	while(itr != _docList.end())
	{
		////////////////////////////////////////////////
		loop ++;
		if(loop >= 10)
			return;
		if(val <= _valMap[*itr])
			itr++;
		else
			break;
	}
	_docList.insert(itr, docId);
}
void Query::result(_DOC_LIST*& docList, _VAL_MAP*& valMap)
{
	docList = &_docList;
	valMap = &_valMap;
}