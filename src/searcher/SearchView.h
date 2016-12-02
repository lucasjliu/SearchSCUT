#ifndef _SEARCHER_H_
#define _SEARCHER_H_

#include "Searcher.h"
#include "Query.h"
#include "Docs.h"

#include <iostream>
#include <fstream>
#include <list>
#include <ctime>
#define PR 0
using namespace std;

class SearchView
{
private:
	Searcher _searcher;
	Docs _docs;
	_OUT_FILE _outFs;
	_PAGERANK _pr;
public:
	SearchView(){}
	SearchView(_PATH dirPath);
	void search(Query&);
	void print(Query&);
	void start();
	static string time_stamp();

};
#endif