#include "Indexer.h"
#include "SearchView.h"
#include <iostream>
using namespace std;

int main()
{
	//Indexer test
	/*cout << SearchView::time_stamp() << " Loading data...\n";
	Indexer indexer("data\\pages\\");
	cout << SearchView::time_stamp() << " Start indexing...\n";
	indexer.start_index();
	cout << SearchView::time_stamp() << " Saving file...\n";
	indexer.save();
	cout << SearchView::time_stamp() << " Success!\n";*/

	/*Query query("student paper");
	Searcher searcher;
	searcher.respond(query);*/

	//Search test
	cout << SearchView::time_stamp() << " Loading data...\n";
	SearchView view("data\\");
	cout << SearchView::time_stamp() << " Success!\n";
	view.start();

	return 0;
}