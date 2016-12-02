#ifndef _INDEXER_H_
#define _INDEXER_H_

#include <vector>
#include <list>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <algorithm>

#include "Links.h"
#include "Docs.h"
#include "Lexicon.h"
#include "StopWords.h"
#include "Index.h"
#include "Config.h"

#define NPOS std::string::npos

extern const string LINKS_FILE;
extern const string INDEX_FILE;
extern const string LEXICON_FILE;
extern const string DOCS_FILE;
extern const string STOPWORD_FILE;
extern const string WORK_DIR;
extern const _WORD_ID noid;

class Indexer
{
private:
	Links _links;
	Docs _docs;
	Lexicon _lexicon;
	Index _index;
	StopWords _stopWords;
	_PATH _repository;
public:
	Indexer(_PATH crawledDir, string workDir = WORK_DIR);
	void start_index();
	static void segment(_TEXT text, _WORD_LIST&, StopWords&);
	void save();
private:
	void get_file(_DOC_ID, _TEXT& filebody);
	void get_content(_TEXT filebody, _TEXT& content);
	void get_title(_TEXT filebody, _TEXT& title);
	void get_url(_DOC_ID, _TEXT&);
	size_t find_html_tag(_TEXT text, _TEXT& tag, size_t pos);
	bool isContent(size_t textLen, size_t tagLen);
	void add_hit(_DOC_ID, _WORD_LIST&, FIELD);
};
#endif