#ifndef _INDEX_H_
#define _INDEX_H_

#include <list>
#include <map>
#include <fstream>

#include "Lexicon.h"
#include "Links.h"
#include "Config.h"

class Index
{	
private:
	_FOWD_INDEX _fowdIndex;
	_INVT_INDEX _invtIndex;
	_FIELD_LENGTH _fieldLength;
	std::string _filePath;
public:
	Index(string);
	Index(){}

	/*
	 * store index to local file. here use file flow but actually binary
	 * file is better
	 */
	bool save();
	/*
	 * add a hit to both fw and iv index
	 */
	void add_hit(_DOC_ID, _WORD_ID, FIELD);

	void get_hit_list(_DOC_ID, _WORD_ID, _HIT_LIST&);
	/*
	 * interface to read the word set of a document
	 */
	void get_doc_index(_DOC_ID docId, _DOC_INDEX&);
	/*
	 * interface to read the document set of a word
	 */
	void get_word_index(_WORD_ID wordId, _WORD_INDEX&);
	/*
	 * read total doc number
	 */
	_INT32 doc_num();
	/*
	 * read doc number in the doc set for a word
	 */
	_INT32 doc_num(_WORD_ID);

	_INT32 word_num();

	_INT32 word_num(_DOC_ID);
	/*
	 * read hit number for a word in a doc
	 */
	_INT32 hit_num(_DOC_ID, _WORD_ID);

	_INT32 field_length(_DOC_ID, FIELD);

private:
	void save_hit_list(_FILE&, _HIT_LIST&);
};
#endif