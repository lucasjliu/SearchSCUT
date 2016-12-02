#include "Searcher.h"

extern const _INT32 FIELD_NUM;;
extern const string INDEX_FILE;
extern const string LEXICON_FILE;
extern const string STOPWORD_FILE;
extern const string WORK_DIR;

Searcher::Searcher(string dirPath = WORK_DIR)
	:_index(dirPath + INDEX_FILE),
	_lexicon(dirPath + LEXICON_FILE),
	_stopWords(dirPath + STOPWORD_FILE)
{
}
void Searcher::respond(Query& query)
{
	_WORD_LIST queryWord;
	segment(query, queryWord, _stopWords);
	_WORD_LIST_ITR wordItr = queryWord.begin();
	while(wordItr != queryWord.end())
	{
		_WORD_ID wordId = _lexicon.get_word_id(*wordItr);
		_WORD_INDEX wordIndex;
		_index.get_word_index(wordId, wordIndex);
		_WORD_INDEX::iterator docItr = wordIndex.begin();
		while(docItr != wordIndex.end())
		{
			_DOC_ID docId = *docItr;
			if(!query.find_doc(docId))
				query.add_doc(docId, score(queryWord, docId));
			++docItr;
		}
		++wordItr;
	}
}
void Searcher::segment(Query& query, _WORD_LIST& queryWords, StopWords& _stopWords)
{
	string q = query.get_query();
	transform(q.begin(), q.end(), q.begin(), ::tolower);
	Indexer::segment(q, queryWords, _stopWords);
}
_VAL Searcher::tf(_WORD_ID wordId, _DOC_ID docId)
{
	return sqrt(_index.hit_num(docId, wordId) * 1.0);
}
_VAL Searcher::idf(_WORD_ID wordId)
{
	return log(_index.doc_num()*1.0/(_index.doc_num(wordId)+1))+1;
}
_VAL Searcher::field_norm(_DOC_ID docId, FIELD field)
{
	_INT32 fieldLength = _index.field_length(docId, field);
	if(!fieldLength)
		return 0;
	return 1.0 / sqrt(fieldLength * 1.0);
}
_VAL Searcher::cord(_DOC_ID docId, _WORD_LIST& queryWords)
{
	_WORD_LIST_ITR itr = queryWords.begin();
	_INT32 num = 0;
	for(; itr != queryWords.end(); ++itr)
	{
		_WORD_ID wordId = _lexicon.get_word_id(*itr);
		_WORD_INDEX wordIndex;
		_index.get_word_index(wordId, wordIndex);
		if(wordIndex.find(docId) != wordIndex.end())
			num++;
	}
	return num*1.0/queryWords.size();
}
_VAL Searcher::weight(_WORD_ID wordId, _DOC_ID docId, _VAL* fieldWeight)
{
	_VAL tfVal = tf(wordId, docId);
	_VAL idfVal = idf(wordId);
	_VAL wordWeight = 0;
	_HIT_LIST hitList;
	_index.get_hit_list(docId, wordId, hitList);
	for(_HIT_LIST_ITR itr = hitList.begin(); itr != hitList.end(); ++itr)
		wordWeight += tfVal * idfVal * fieldWeight[itr->_field];
	//printf("word %2d, doc %2d :  %4.2f  %4.2f  %4.2f  %4.2f  %4.2f\n", wordId, docId, tfVal, idfVal, fieldWeight[0], fieldWeight[1], fieldWeight[2]);
	return wordWeight;
}
_VAL Searcher::score(_WORD_LIST& queryWords, _DOC_ID docId)
{
	_VAL docScore = 0;
	_VAL* fieldWeight = (_VAL*)calloc(FIELD_NUM, sizeof(_VAL));

	//compute fieldWeight = fieldBoost / sqrt(fieldLength)
	for(_INT32 field = 0; field < FIELD_NUM; ++field)
		fieldWeight[field]=field_boost((FIELD)field) * field_norm(docId, (FIELD)field);

	for(_WORD_LIST_ITR itr = queryWords.begin(); itr != queryWords.end(); ++itr)
		docScore += weight(_lexicon.get_word_id(*itr), docId, fieldWeight);

	delete []fieldWeight;
	return cord(docId, queryWords) * docScore;
}
_VAL Searcher::field_boost(FIELD field)
{
	return 1.0 + field * 0.5;
}