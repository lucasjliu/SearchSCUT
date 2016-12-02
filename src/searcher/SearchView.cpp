#include "SearchView.h"

extern const string WORK_DIR;
extern const string RESULT_FILE;
extern const string PAGERANK_FILE;
extern const string DOCS_FILE;


SearchView::SearchView(_PATH dirPath = WORK_DIR)
	:_outFs(dirPath+RESULT_FILE, ios::app),
	_docs(dirPath + DOCS_FILE),
	_searcher(dirPath)
{
	_IN_FILE prFs(dirPath + PAGERANK_FILE);
	_DOC_ID docId; double val;
	if(PR)
		while(prFs >> val >> docId)
			_pr[docId] = val;
}
void SearchView::search(Query& query)
{
	_searcher.respond(query);
	cout << SearchView::time_stamp() << " Success!\n\n";
	print(query);
	cout << endl;
	_outFs << endl;
}
void SearchView::print(Query& query)
{
	char qid[6] = {0};
	sprintf(qid, "TD%03d", query.get_id());//////////////////////////////////
	cout << qid << ": " << query.get_query() << endl;
	_outFs << qid << ": " << query.get_query() << endl;

	_DOC_LIST* docList = NULL;
	_VAL_MAP* valMap = NULL;
	query.result(docList, valMap);
	if(docList == NULL || valMap == NULL)
		return;
	if(!docList->size() || !valMap->size())
		return;
	for(int i = 0; i < 10; i++)
	{
		_DOC_LIST_ITR itr = docList->begin();
		_DOC_LIST_ITR max = itr;
		if(_pr.size())
		{
			while(++itr != docList->end())
				if(_pr[*itr] > _pr[*max])
					max = itr;
		}
		cout << _docs.get_info(*max) << "  " << (*valMap)[*max] << endl;
		_outFs << _docs.get_info(*max) << "  " << (*valMap)[*max] << endl;
		docList->erase(max);
		if(docList->size() == 0)
			return;
	}
}
void SearchView::start()
{
	_outFs << "-----------------------------------------------\n";
	int N;
	cout << "> Number of test cases: ";
	cin >> N;
	for(int i = 1; i <= N; i++)
	{
		string str;
		cout << "> Input a query string: ";
		cin.sync();
		getline(cin, str);
		if(str.length() > 64 || str.length() == 0)
		{
			cout << "error: query null or length exceed. try again\n";
			--i;
			continue;
		}
		Query query(str, i);
		cout << SearchView::time_stamp() << " responding...\n";
		search(query);
	}
}
string SearchView::time_stamp()
{
	time_t t=time(NULL);
	char buf[32];
	strftime(buf,sizeof(buf),"[%H:%M:%S]",localtime(&t));
	return buf;
}