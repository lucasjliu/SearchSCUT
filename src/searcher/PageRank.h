#ifndef _PAGERANK_H_
#define _PAGERANK_H_

#include <map>
#include <list>
#include <iostream>
#include <fstream>
using namespace std;

class PageRank
{
private:
	// Map from doc to all links outbound
	map<int, list<int> > out;
	// Map from doc to all links inbound
	map<int, list<int> > in;
	// Final computed pagerank
	map<int, double> pr;

public:
	PageRank();
	void put(int docId, list<int>& docIds );
	void compute();
	double get(int docId);
	bool output(const char* file);

private:
	void buildInbound();
};

#endif

