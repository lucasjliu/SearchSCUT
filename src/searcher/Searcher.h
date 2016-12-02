#include "Indexer.h"
#include "Query.h"
#include "Config.h"

#include <math.h>
#include <iostream>

class Searcher
{
private:
	Index _index;

	Lexicon _lexicon;

	StopWords _stopWords;

public:
	Searcher(){}
	Searcher(string);
	/*
	 * find relevant document and similarity ranking
	 * pass to user view by Query object
	 */
	void respond(Query&);
private:
	/*
	 * segment the query string. call Indexer::segment()
	 */
	void segment(Query&, _WORD_LIST&, StopWords&);
	/*
	 * tf(i,j) = sqrt(freq of word i in document j)
	 */
	_VAL tf(_WORD_ID, _DOC_ID);
	/*
	 * idf(word i) = 
	 * ln(total word num / (num of doc that include i) + 1) + 1
	 */
	_VAL idf(_WORD_ID);
	/*
	 * unnecessary for sorting. not used in this project
	 */
	_VAL query_norm(_WORD_LIST&){}
	/*
	 * fieldNorm(field type in doc j) = 
	 * 1/sqrt(num of this type of hit in doc j)
	 */
	_VAL field_norm(_DOC_ID, FIELD);
	/*
	 * weight(t, j): sum of the word hit weight in a document
	 * for each word in a doc, weight = tf * idf * fieldWeight
	 * fieldWeight(field) = fieldBoost * fieldNorm, cal in score()
	 */
	_VAL weight(_WORD_ID, _DOC_ID, _VAL*);
	/*
	 * cord for doc = query word matched/total query words num
	 */
	_VAL cord(_DOC_ID, _WORD_LIST&);
	/*
	 * final score for a document j = 
	 * cord(j) * sum of each query word weight in j
	 */
	_VAL score(_WORD_LIST&, _DOC_ID);
	/*
	 * measures contribution of different field.
	 * default: content = 1.0, title = 1.5, url = 2.0
	 */
	_VAL field_boost(FIELD);
};