#ifndef _QUERY_H_
#define _QUERY_H_

#include "Docs.h"
#include "Config.h"

#include <list>
#include <string>
#include <map>

using namespace std;

class Query
{
private:
	string _query;
	int _qid;
	_DOC_LIST _docList;
	_VAL_MAP _valMap;
public:
	Query(string, int);
	Query();
	string get_query() const;
	int get_id() const;
	void add_doc(_DOC_ID, double);
	bool find_doc(_DOC_ID);
	void result(_DOC_LIST*&, _VAL_MAP*&);
private:
	
};
#endif